#ifndef GLCONTEXT_H_
#define GLCONTEXT_H_

#include <Egl.h>
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "android/log.h"

#include "JNIHelper.h"

MACE_START

/******************************************************************
 * OpenGL context handler
 * The class handles OpenGL and EGL context based on Android activity life cycle
 * The caller needs to call corresponding methods for each activity life cycle
 *events as it's done in sample codes.
 *
 * Also the class initializes OpenGL ES3 when the compatible driver is installed
 *in the device.
 * getGLVersion() returns 3.0~ when the device supports OpenGLES3.0
 *
 * Thread safety: OpenGL context is expecting used within dedicated single
 *thread,
 * thus GLContext class is not designed as a thread-safe
 */
class GLContext {
public:

    GLContext();
    GLContext(GLContext const&) = delete;
    void operator = (GLContext const&) = delete;
    virtual ~GLContext();

    void init(ANativeWindow* window);
    void suspend();
    void resume(ANativeWindow* window);

    void swap();

    ANativeWindow* getWindow() const { return window; };
    int32_t getScreenWidth() const { return egl.getWidth(); }
    int32_t getScreenHeight() const { return egl.getHeight(); }
    bool isInitialized() const { return initialized; }
    float getGLVersion() const { return glVersion; }

private:
    Egl egl;
    ANativeWindow* window;
    float glVersion;
    bool initialized;

    void terminate();
};

MACE_END

#endif /* GLCONTEXT_H_ */
