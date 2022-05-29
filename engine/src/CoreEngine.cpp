//
// Created by Brandon on 5/13/22.
//

#include "CoreEngine.h"
#include "engine/EngineImpl.h"
#include "PlatformEngine.h"

MACE_START

PlatformEngine* CoreEngine::deviceEngine;

EngineImpl& CoreEngine::Instance() {
    static EngineImpl engine{std::unique_ptr<PlatformEngine>(deviceEngine)};   // the singleton from which all other Engine systems belong to
    return engine;
}

void CoreEngine::Launch() {
    Instance().launch();
}

EventSystem& CoreEngine::EventSystem() {
    return Instance().getEventSystem();
}

MACE_END