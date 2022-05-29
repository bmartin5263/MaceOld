//
// Created by Brandon on 5/15/22.
//

#ifndef MACE_ENGINEIMPL_H
#define MACE_ENGINEIMPL_H

#ifdef __ANDROID__
struct android_app;
#endif

#include "graphics/GLContext.h"
#include "Clock.h"
#include "rendering/TexturedTeapotRenderer.h"
#include "rendering/TapCamera.h"
#include "EventSystem.h"

#include "Core.h"

MACE_START

class PlatformEngine;
class EngineImpl {
public:
    explicit EngineImpl(std::unique_ptr<PlatformEngine> deviceEngine);
    void launch();

    EventSystem& getEventSystem();

private:
    void update();
    void draw();
    bool isRunning();

    std::unique_ptr<PlatformEngine> deviceEngine;
    EventSystem eventSystem;

    TapCamera tapCamera;
    GLContext glContext;
    Clock engineClock;

    TexturedTeapotRenderer teapotRender;

};

MACE_END

#endif //MACE_ENGINEIMPL_H
