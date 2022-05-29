//
// Created by Brandon on 5/22/22.
//

#ifndef MACE_EGL_H
#define MACE_EGL_H

#include <EGL/egl.h>
#include "Core.h"

MACE_START

class Egl {
public:
    void init(ANativeWindow* window);
    void resume(ANativeWindow* window);
    void swap();
    void suspend();
    void terminate();

    int32_t getWidth() const { return width; }
    int32_t getHeight() const { return height; }
    int32_t getBufferColorSize() const { return colorSize; }
    int32_t getBufferDepthSize() const { return depthSize; }

private:
    EGLDisplay display;         // created when starting EGL
    EGLConfig config;           // config stuff idk
    EGLSurface surface;         // has width/height
    EGLContext context;

    int32_t width;
    int32_t height;
    int32_t colorSize;
    int32_t depthSize;

    EGLConfig createConfig(EGLDisplay display);
    EGLSurface createSurface(EGLDisplay display, EGLConfig config, EGLNativeWindowType window);
    static void makeContextCurrent(EGLDisplay display, EGLSurface surface, EGLContext context);

    static constexpr EGLint OPEN_GL_3_ATTRIBUTE[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };

    static constexpr EGLint DEPTH_24_BIT_ATTRIBUTE[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };

    static constexpr EGLint DEPTH_16_BIT_ATTRIBUTE[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_NONE
    };
};

MACE_END

#endif //MACE_EGL_H
