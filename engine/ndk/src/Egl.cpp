//
// Created by Brandon on 5/22/22.
//

#include "Egl.h"
#include "Log.h"
#include "Core.h"

MACE_START

constexpr EGLint Egl::OPEN_GL_3_ATTRIBUTE[];
constexpr EGLint Egl::DEPTH_24_BIT_ATTRIBUTE[];
constexpr EGLint Egl::DEPTH_16_BIT_ATTRIBUTE[];

void Egl::init(ANativeWindow *window) {
    INFO("Starting EGL");

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);

    config = createConfig(display);
    surface = createSurface(display, config, window);

    auto context = eglCreateContext(display, config, nullptr, OPEN_GL_3_ATTRIBUTE);

    makeContextCurrent(display, surface, context);
}

void Egl::resume(ANativeWindow *window) {
    INFO("Resuming EGL");

    surface = createSurface(display, config, window);
    makeContextCurrent(display, surface, context);
}

void Egl::suspend() {
    INFO("Suspending EGL");

    eglDestroySurface(display, surface);
    surface = EGL_NO_SURFACE;
}

void Egl::terminate() {
    INFO("Terminating EGL");

    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }

        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

EGLSurface Egl::createSurface(EGLDisplay display, EGLConfig config, EGLNativeWindowType window) {
    DBUG("Creating EGL Surface");
    auto surface = eglCreateWindowSurface(display, config, window, nullptr);
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);
    return surface;
}

EGLConfig Egl::createConfig(EGLDisplay display) {
    EGLint configCount;
    eglChooseConfig(display, DEPTH_24_BIT_ATTRIBUTE, &config, 1, &configCount);
    this->depthSize = 24;
    this->colorSize = 8;
    if (configCount == 0) {
        eglChooseConfig(display, DEPTH_16_BIT_ATTRIBUTE, &config, 1, &configCount);
        this->depthSize = 16;
        if (configCount == 0) {
            ERROR("Unable to retrieve EGL config");
            ASSERT(false, "Unable to retrieve EGL config")
        }
    }

    return config;
}

void Egl::makeContextCurrent(EGLDisplay display, EGLSurface surface, EGLContext context) {
    DBUG("Setting EGL Context");
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        ASSERT(false, "Unable to eglMakeCurrent")
    }
}

void Egl::swap() {
    bool success = eglSwapBuffers(display, surface);
    if (!success) {
        ASSERT(false, "Failed to swap buffer")
    }
}

MACE_END