//
// Created by Brandon on 5/15/22.
//

#include "EventSystem.h"
#include "CoreEngine.h"

MACE_START

EventSystem &EventSystem::Instance() {
    return CoreEngine::EventSystem();
}

void EventSystem::pushEvent(const Event &event) {
    eventQueue.push(event);
}

void EventSystem::PushEvent(const Event &event) {
    Instance().pushEvent(event);
}

MACE_END
