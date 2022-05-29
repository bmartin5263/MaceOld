#include <cstdlib>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "graphics/Shader.h"

MACE_START

//#define SHADER_DEBUG (0)

bool CompileShader(
    GLuint *shader, const GLenum type, const char *str_file_name,
    const std::map<std::string, std::string> &map_parameters) {
  std::vector<uint8_t> data;
  if (!mace::ndk::JNIHelper::GetInstance()->ReadFile(str_file_name, &data)) {
    INFO("Can not open a file:%s", str_file_name);
    return false;
  }

  const char REPLACEMENT_TAG = '*';
  // Fill-in parameters
  std::string str(data.begin(), data.end());
  std::string str_replacement_map(data.size(), ' ');

  auto it = map_parameters.begin();
  auto itEnd = map_parameters.end();
  while (it != itEnd) {
    size_t pos = 0;
    while ((pos = str.find(it->first, pos)) != std::string::npos) {
      // Check if the sub string is already touched

      size_t replaced_pos = str_replacement_map.find(REPLACEMENT_TAG, pos);
      if (replaced_pos == std::string::npos || replaced_pos > pos) {
        str.replace(pos, it->first.length(), it->second);
        str_replacement_map.replace(pos, it->first.length(), it->first.length(),
                                    REPLACEMENT_TAG);
        pos += it->second.length();
      } else {
        // The replacement target has been touched by other tag, skipping them
        pos += it->second.length();
      }
    }
    it++;
  }

  INFO("Patched Shdader:\n%s", str.c_str());

  std::vector<uint8_t> v(str.begin(), str.end());
  str.clear();
  return CompileShader(shader, type, v);
}

bool CompileShader(GLuint *shader, const GLenum type,
                           const GLchar *source, const int32_t iSize) {
  if (source == NULL || iSize <= 0) return false;

  *shader = glCreateShader(type);
  glShaderSource(*shader, 1, &source, &iSize);  // Not specifying 3rd parameter
                                                // (size) could be troublesome..

  glCompileShader(*shader);

#if defined(SHADER_DEBUG)
  GLint logLength;
  glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    GLchar *log = (GLchar *)malloc(logLength);
    glGetShaderInfoLog(*shader, logLength, &logLength, log);
    INFO("Shader compile log:\n%s", log);
    free(log);
  }
#endif

  GLint status;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
  if (status == 0) {
    glDeleteShader(*shader);
    return false;
  }

  return true;
}

bool CompileShader(GLuint *shader, const GLenum type,
                           std::vector<uint8_t> &data) {
  if (!data.size()) return false;

  const GLchar *source = (GLchar *)&data[0];
  int32_t iSize = data.size();
  return CompileShader(shader, type, source, iSize);
}

bool CompileShader(GLuint *shader, const GLenum type,
                           const char *strFileName) {
  std::vector<uint8_t> data;
  bool b = mace::ndk::JNIHelper::GetInstance()->ReadFile(strFileName, &data);
  if (!b) {
    INFO("Can not open a file:%s", strFileName);
    return false;
  }

  return CompileShader(shader, type, data);
}

bool LinkProgram(const GLuint prog) {
  GLint status;

  glLinkProgram(prog);

#if defined(SHADER_DEBUG)
  GLint logLength;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    GLchar *log = (GLchar *)malloc(logLength);
    glGetProgramInfoLog(prog, logLength, &logLength, log);
    INFO("Program link log:\n%s", log);
    free(log);
  }
#endif

  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (status == 0) {
    INFO("Program link failed\n");
    return false;
  }

  return true;
}

bool ValidateProgram(const GLuint prog) {
  GLint logLength, status;

  glValidateProgram(prog);
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    GLchar *log = (GLchar *)malloc(logLength);
    glGetProgramInfoLog(prog, logLength, &logLength, log);
    INFO("Program validate log:\n%s", log);
    free(log);
  }

  glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
  if (status == 0) return false;

  return true;
}

MACE_END
