/*
 * Copyright 2013 The Android Open Source Project
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

#include "graphics/GLContext.h"

#include <cstring>
#include <unistd.h>

#include "graphics/gl3stub.h"
#include "Log.h"

MACE_START

GLContext::GLContext():
    window(nullptr),
    initialized(false)
{}

GLContext::~GLContext() {
    INFO("~GLContext()");
    terminate();
}

void GLContext::init(ANativeWindow* window) {
    INFO("Starting OpenGL ES");
    if (initialized) {
        return;
    }

    this->window = window;
    egl.init(window);

    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(0, 0, egl.getWidth(), egl.getHeight());

    initialized = true;
}

void GLContext::resume(ANativeWindow *window) {
    INFO("Resuming OpenGL ES");
    this->window = window;
    egl.resume(window);

    // Note that screen size might have been changed
    glViewport(0, 0, egl.getWidth(), egl.getHeight());
}

void GLContext::swap() {
    egl.swap();
}

void GLContext::terminate() {
    egl.terminate();
    window = nullptr;
    initialized = false;
}

void GLContext::suspend() {
    egl.suspend();
}

MACE_END
