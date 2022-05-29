/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "rendering/TeapotRenderer.h"
#include "rendering/teapot.inl"
#include "graphics/gl3stub.h"
#include <cassert>
#include "graphics/Shader.h"
#include "rendering/TapCamera.h"
#include "Log.h"

MACE_START

TeapotRenderer::~TeapotRenderer() {
    destroy();
}

void TeapotRenderer::init() {
    // Settings
    glFrontFace(GL_CCW);

    // Load shader
    GLint type = getTextureType();
    if (type == GL_TEXTURE_CUBE_MAP) {
        LoadShaders(&shaderParams, "Shaders/Cubemap.vsh","Shaders/Cubemap.fsh");
    } else if (type == GL_TEXTURE_2D) {
        LoadShaders(&shaderParams, "Shaders/2DTexture.vsh","Shaders/2DTexture.fsh");
    } else {
        LoadShaders(&shaderParams, "Shaders/VS_ShaderPlain.vsh","Shaders/ShaderPlain.fsh");
    }
    // Create Index buffer
    numIndices = sizeof(teapotIndices) / sizeof(teapotIndices[0]);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create VBO
    numVertices = sizeof(teapotPositions) / sizeof(teapotPositions[0]) / 3;
    int32_t stride = sizeof(TeapotVertex);
    int32_t index = 0;
    auto* p = new TeapotVertex[numVertices];
    for (int32_t i = 0; i < numVertices; ++i) {
        p[i].pos[0] = teapotPositions[index];
        p[i].pos[1] = teapotPositions[index + 1];
        p[i].pos[2] = teapotPositions[index + 2];

        p[i].normal[0] = teapotNormals[index];
        p[i].normal[1] = teapotNormals[index + 1];
        p[i].normal[2] = teapotNormals[index + 2];
        index += 3;
    }
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, stride * numVertices, p, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] p;

    updateViewport();
    model = Mat4::Translation(0, 0, -15.f);

    Mat4 mat = Mat4::RotationX(M_PI / 3);
    model = mat * model;
}

void TeapotRenderer::updateViewport() {
    // Init Projection matrices
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    const float CAM_NEAR = 5.f;
    const float CAM_FAR = 10000.f;
    if (viewport[2] < viewport[3]) {
        float aspect =
            static_cast<float>(viewport[2]) / static_cast<float>(viewport[3]);
        projection =
            Mat4::Perspective(aspect, 1.0f, CAM_NEAR, CAM_FAR);
    } else {
        float aspect =
            static_cast<float>(viewport[3]) / static_cast<float>(viewport[2]);
        projection =
            Mat4::Perspective(1.0f, aspect, CAM_NEAR, CAM_FAR);
    }
}

void TeapotRenderer::destroy() {
    if (vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (ibo) {
        glDeleteBuffers(1, &ibo);
        ibo = 0;
    }

    if (shaderParams.program) {
        glDeleteProgram(shaderParams.program);
        shaderParams.program = 0;
    }
}

void TeapotRenderer::update(float fTime) {
    const float CAM_X = 0.f;
    const float CAM_Y = 0.f;
    const float CAM_Z = 700.f;

    view = Mat4::LookAt(Vec3(CAM_X, CAM_Y, CAM_Z),
                        Vec3(0.f, 0.f, 0.f),
                        Vec3(0.f, 1.f, 0.f));

    if (camera) {
        camera->update();
        view = camera->getTransformMatrix() * view * camera->getRotationMatrix() * model;
    } else {
        view = view * model;
    }
}

void TeapotRenderer::render() {
    // Feed Projection and Model View matrices to the shaders
    Mat4 mat_vp = projection * view;

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int32_t iStride = sizeof(TeapotVertex);
    // Pass the vertex data
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, iStride,
                          BUFFER_OFFSET(0));
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, iStride,
                          BUFFER_OFFSET(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_NORMAL);

    // Bind the IB
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glUseProgram(shaderParams.program);

    TeapotMaterials material = {
        {1.0f, 0.5f, 0.5f},
        {1.0f, 1.0f, 1.0f, 10.f},
        {0.1f, 0.1f, 0.1f},
    };

    // Update uniforms
    glUniform4f(shaderParams.materialDiffuse, material.diffuseColor[0],
                material.diffuseColor[1], material.diffuseColor[2], 1.f);

    glUniform4f(shaderParams.materialSpecular, material.specularColor[0],
                material.specularColor[1], material.specularColor[2],
                material.specularColor[3]);
    //
    // using glUniform3fv here was troublesome
    //
    glUniform3f(shaderParams.materialAmbient, material.ambientColor[0],
                material.ambientColor[1], material.ambientColor[2]);

    glUniformMatrix4fv(shaderParams.matrixProjection, 1, GL_FALSE,
                       mat_vp.Ptr());
    glUniformMatrix4fv(shaderParams.matrixView, 1, GL_FALSE, view.Ptr());
    glUniform3f(shaderParams.light0, 100.f, -200.f, -600.f);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,
                   BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool TeapotRenderer::LoadShaders(ShaderParams *params, const char *strVsh,
                                 const char *strFsh) {
    GLuint program;
    GLuint vert_shader, frag_shader;

    // Create shader program
    program = glCreateProgram();
    INFO("Created Shader %d", program);

    // Create and compile vertex shader
    if (!CompileShader(&vert_shader, GL_VERTEX_SHADER,
                                           strVsh)) {
        INFO("Failed to compile vertex shader");
        glDeleteProgram(program);
        assert(false);
        return false;
    }

    // Create and compile fragment shader
    if (!CompileShader(&frag_shader, GL_FRAGMENT_SHADER,
                                           strFsh)) {
        INFO("Failed to compile fragment shader");
        glDeleteProgram(program);
        assert(false);
        return false;
    }

    // Attach vertex shader to program
    glAttachShader(program, vert_shader);

    // Attach fragment shader to program
    glAttachShader(program, frag_shader);

    // Bind attribute locations
    // this needs to be done prior to linking
    glBindAttribLocation(program, ATTRIB_VERTEX, "myVertex");
    glBindAttribLocation(program, ATTRIB_NORMAL, "myNormal");
    glBindAttribLocation(program, ATTRIB_UV, "myUV");

    // Link program
    if (!LinkProgram(program)) {
        INFO("Failed to link program: %d", program);

        if (vert_shader) {
            glDeleteShader(vert_shader);
            vert_shader = 0;
        }
        if (frag_shader) {
            glDeleteShader(frag_shader);
            frag_shader = 0;
        }
        if (program) {
            glDeleteProgram(program);
        }

        return false;
    }

    // Get uniform locations
    params->matrixProjection = glGetUniformLocation(program, "uPMatrix");
    params->matrixView = glGetUniformLocation(program, "uMVMatrix");

    params->light0 = glGetUniformLocation(program, "vLight0");
    params->materialDiffuse = glGetUniformLocation(program, "vMaterialDiffuse");
    params->materialAmbient = glGetUniformLocation(program, "vMaterialAmbient");
    params->materialSpecular =
        glGetUniformLocation(program, "vMaterialSpecular");

    // Release vertex and fragment shaders
    if (vert_shader) glDeleteShader(vert_shader);
    if (frag_shader) glDeleteShader(frag_shader);

    params->program = program;
    return true;
}

void TeapotRenderer::bind(TapCamera *camera) {
    this->camera = camera;
}

MACE_END