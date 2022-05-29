//
// Created by Brandon on 5/12/22.
//

#ifndef MACE_ORIGINALENGINE_H
#define MACE_ORIGINALENGINE_H

#include <jni.h>
#include <cerrno>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include "graphics/GLContext.h"
#include "Clock.h"
#include "Core.h"

#include "rendering/TexturedTeapotRenderer.h"
#include "NDKHelper.h"
#include "PinchDetector.h"
#include "DoubleTapDetector.h"
#include "DragDetector.h"

#include "rendering/TapCamera.h"

class OriginalEngine {
public:
    explicit OriginalEngine(android_app *app);

    void run();
    bool update();
    void draw();

    int initWindow(android_app *app);
    void loadResources();
    void unloadResources();
    void drawFrame();
    void termWindow();
    bool hasFocus() const;

    void handleCmd(android_app* app, int32_t cmd);
    int32_t handleInputEvent(android_app* app, AInputEvent* event);

    static void OnAppCmd(android_app* app, int32_t cmd);
    static int32_t OnInputEvent(android_app* app, AInputEvent* event);

private:
    void updateFPS();

    void toGlCoordinates(mace::Vec2 &vec);
    inline void toGlCoordinates(mace::Vec2 &v1, mace::Vec2 &v2);

    // Input handling
    void handleDrag(AInputEvent *event);
    void handlePinch(AInputEvent *event);

    bool focused;

    android_app* app;

    mace::TapCamera tapCamera;
    mace::GLContext glContext;
    mace::Clock engineClock;

    mace::TexturedTeapotRenderer teapotRender;

    mace::ndk::DoubleTapDetector doubleTapDetector;
    mace::ndk::DragDetector dragDetector;
    mace::ndk::PinchDetector pinchDetector;
    mace::ndk::SensorManager sensorManager;
};

#endif //MACE_ORIGINALENGINE_H
