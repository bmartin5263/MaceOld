//
// Created by Brandon on 5/18/22.
//

#ifndef MACE_PLATFORMENGINE_H
#define MACE_PLATFORMENGINE_H

#ifdef __ANDROID__
#include <android_native_app_glue.h>
#include "NDKHelper.h"
#include "DoubleTapDetector.h"
#include "PinchDetector.h"
#include "DragDetector.h"
#endif

MACE_START

class EngineImpl;
class PlatformEngine {
public:
    void launch(EngineImpl* engine);

#ifdef __ANDROID__
public:
    PlatformEngine(android_app* androidApp);
private:
    android_app* app;
    mace::ndk::DoubleTapDetector doubleTapDetector;
    mace::ndk::DragDetector dragDetector;
    mace::ndk::PinchDetector pinchDetector;
    mace::ndk::SensorManager sensorManager;
#endif


};

MACE_END

#endif //MACE_PLATFORMENGINE_H
