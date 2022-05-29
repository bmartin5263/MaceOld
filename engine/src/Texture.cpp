#include "graphics/Texture.h"
#include <GLES3/gl32.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "AssetUtil.h"

#include "android_debug.h"
#include "JNIHelper.h"

MACE_START

/**
 * Cubemap and Texture2d implementations for Class Texture.
 */
class TextureCubemap : public Texture {
protected:
    GLuint texId_ = GL_INVALID_VALUE;
    bool activated_ = false;

public:
    ~TextureCubemap() override;
    TextureCubemap(std::vector<std::string>& texFiles,
                   AAssetManager* assetManager);
    bool GetActiveSamplerInfo(std::vector<std::string>& names,
                                      std::vector<GLint>& units) override;
    bool Activate() override;
    GLuint GetTexType() override;
    GLuint GetTexId() override;
};

class Texture2d :public Texture {
protected:
    GLuint texId_ = GL_INVALID_VALUE;
    bool activated_ = false;
public:
    ~Texture2d() override;
    // Implement just one texture
    Texture2d(std::string &texFiles, AAssetManager* assetManager);
    bool GetActiveSamplerInfo(std::vector<std::string>& names, std::vector<GLint>& units) override;
    bool Activate() override;
    GLuint GetTexType() override;
    GLuint GetTexId() override;
};

static const std::string supportedTextureTypes = "GL_TEXTURE_2D(0x0DE1) GL_TEXTURE_CUBE_MAP(0x8513)";

/**
 * Create Texture Object
 * @param texFiles holds the texture file name(s) under APK's assets
 * @param type should be one (GL_TEXTURE_2D / GL_TEXTURE_CUBE_MAP)
 * @param assetManager is used to open texture files inside assets
 * @return is the newly created Texture Object
 */
Texture* Texture::Create( GLuint type, std::vector<std::string>& texFiles,
                       AAssetManager* assetManager) {
    if (type == GL_TEXTURE_2D) {
        return dynamic_cast<Texture*>(new Texture2d(texFiles[0], assetManager));
    } else if (type == GL_TEXTURE_CUBE_MAP) {
        return dynamic_cast<Texture*>(new TextureCubemap(texFiles, assetManager));
    }

    ERROR("Unknow texture type %x to created", type);
    ERROR("Supported Texture Types: %s", supportedTextureTypes.c_str());
    assert(false);
    return nullptr;
}

/**
 * TextureCubemap implementations
 */
bool TextureCubemap::Activate() {
    assert(texId_ != GL_INVALID_VALUE);

    glBindTexture(texId_, GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0 + 0);
    activated_ = true;
    return true;
}

GLuint TextureCubemap::GetTexType() {
    return GL_TEXTURE_CUBE_MAP;
}

GLuint TextureCubemap::GetTexId() {
    assert(texId_ != GL_INVALID_VALUE);
    return texId_;
}

TextureCubemap::TextureCubemap(std::vector<std::string> &files,
                                    AAssetManager *mgr) {
    // For Cubemap, we use world normal to sample the textures
    // so no texture vbo necessary

    int32_t imgWidth, imgHeight, channelCount;
    std::vector<uint8_t> fileBits;

    if (!mgr || files.size() != 6) {
        assert(false);
        return;
    }

    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId_);

    if(texId_ == GL_INVALID_VALUE) {
        assert(false);
        return;
    }

    for(GLuint i = 0; i < 6; i++) {
        fileBits.clear();
        mace::ndk::AssetReadFile(mgr, files[i], fileBits);

        // tga/bmp files are saved as vertical mirror images ( at least more than half ).
        stbi_set_flip_vertically_on_load(1);

        uint8_t* imageBits = stbi_load_from_memory(
            fileBits.data(), fileBits.size(),
            &imgWidth, &imgHeight, &channelCount, 4);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGBA,
                     imgWidth, imgHeight,
                     0, GL_RGBA,
                     GL_UNSIGNED_BYTE, imageBits);
        stbi_image_free(imageBits);
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT );

    glActiveTexture(GL_TEXTURE0);
    activated_ = true;
}

/**
 * Dtor
 *    clean-up function
 */
TextureCubemap::~TextureCubemap() {
    if (texId_!= GL_INVALID_VALUE) {
        glDeleteTextures(1, &texId_);
        texId_ = GL_INVALID_VALUE;
        activated_ =  false;
    }
}

/**
  Return used sampler names and units
      so application could configure shader's sampler uniform(s).
  Cubemap just used one sampler at unit 0 with "samplerObj" as its name.
 */
 bool TextureCubemap::GetActiveSamplerInfo(std::vector<std::string>& names,
                                           std::vector<GLint>& units) {
    names.clear();
    names.push_back(std::string("samplerObj"));
    units.clear();
    units.push_back(0);

    return true;
}

/**
 * Texture2D implementation
 */
Texture2d::Texture2d(std::string& fileName, AAssetManager* assetManager)  {
    if (!assetManager) {
        ERROR("AssetManager to Texture2D() could not be null!!!");
        assert(false);
        return;
    }

    int32_t imgWidth, imgHeight, channelCount;
    std::string texName(fileName);
    std::vector<uint8_t> fileBits;

    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_2D, texId_);

    if(texId_ == GL_INVALID_VALUE) {
        assert(false);
        return;
    }

    mace::ndk::AssetReadFile(assetManager, texName, fileBits);

    // tga/bmp files are saved as vertical mirror images ( at least more than half ).
    stbi_set_flip_vertically_on_load(1);

    uint8_t* imageBits = stbi_load_from_memory(
            fileBits.data(), fileBits.size(),
            &imgWidth, &imgHeight, &channelCount, 4);
    glTexImage2D(GL_TEXTURE_2D, 0,  // mip level
                 GL_RGBA,
                 imgWidth, imgHeight,
                 0,                // border color
                 GL_RGBA, GL_UNSIGNED_BYTE, imageBits);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glActiveTexture(GL_TEXTURE0);

    stbi_image_free(imageBits);
}

Texture2d::~Texture2d() {
    if (texId_!= GL_INVALID_VALUE) {
        glDeleteTextures(1, &texId_);
        texId_ = GL_INVALID_VALUE;
    }
    activated_ = false;
}

/**
 * Same as the Cubemap::GetActiveSamplerInfo()
 */
bool Texture2d::GetActiveSamplerInfo(std::vector<std::string>& names,
                                      std::vector<GLint>& units) {
    names.clear();
    names.push_back(std::string("samplerObj"));
    units.clear();
    units.push_back(0);

    return true;
}

bool Texture2d::Activate(void) {
    glBindTexture(texId_, GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0 + 0);
    activated_ = true;
    return true;
}

GLuint Texture2d::GetTexType() {
    return GL_TEXTURE_2D;
}

GLuint Texture2d::GetTexId() {
    return texId_;
}

MACE_END
