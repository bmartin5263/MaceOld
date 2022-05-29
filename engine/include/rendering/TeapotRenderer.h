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

#ifndef _TEAPOTRENDERER_H
#define _TEAPOTRENDERER_H

#include <jni.h>
#include <errno.h>

#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <math/vecmath.h>
#include <android/asset_manager.h>

//#include <android/sensor.h>
//#include <android/log.h>
//#include <android_native_app_glue.h>
//#include <android/native_window_jni.h>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

MACE_START

struct TeapotVertex {
    float pos[3];
    float normal[3];
};

enum ShaderAttributes {
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    ATTRIB_UV,
};

struct ShaderParams {
    GLuint program;
    GLint light0;
    GLint materialDiffuse;
    GLint materialAmbient;
    GLint materialSpecular;

    GLint matrixProjection;
    GLint matrixView;
};

struct TeapotMaterials {
    float diffuseColor[3];
    float specularColor[4];
    float ambientColor[3];
};

class TapCamera;
class TeapotRenderer {
public:
    TeapotRenderer() = default;
    virtual ~TeapotRenderer();

    virtual void init(AAssetManager *amgr) = 0;
    virtual GLint getTextureType() = 0;

    virtual void render();
    virtual void destroy();

    void update(float dTime);
    void bind(TapCamera* camera);
    void updateViewport();

protected:
    static bool LoadShaders(ShaderParams* params, const char* strVsh, const char* strFsh);

    void init();

    int32_t numIndices;
    int32_t numVertices;

    GLuint ibo;
    GLuint vbo;

    ShaderParams shaderParams;

    Mat4 projection;
    Mat4 view;
    Mat4 model;

    TapCamera* camera;
};

MACE_END

#endif
