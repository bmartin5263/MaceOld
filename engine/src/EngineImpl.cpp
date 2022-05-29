//
// Created by Brandon on 5/15/22.
//

#include "engine/EngineImpl.h"
#include "PlatformEngine.h"
#include "EventSystem.h"
#include "Log.h"

MACE_START

EngineImpl::EngineImpl(std::unique_ptr<PlatformEngine> deviceEngine):
    deviceEngine{std::move(deviceEngine)}, eventSystem{}, tapCamera{}, glContext{},
    engineClock{}, teapotRender{}
{
}

void EngineImpl::launch() {

    update();
    draw();
}

void EngineImpl::update() {
    ERROR("EngineImpl::update()");
}

void EngineImpl::draw() {
    ERROR("EngineImpl::draw()");
}

bool EngineImpl::isRunning() {
    return true;
}

EventSystem &EngineImpl::getEventSystem() {
    return eventSystem;
}

MACE_END