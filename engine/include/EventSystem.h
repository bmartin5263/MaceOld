//
// Created by Brandon on 5/15/22.
//

#ifndef MACE_EVENTSYSTEM_H
#define MACE_EVENTSYSTEM_H

#include <queue>
#include "events/Event.h"
#include "Core.h"

MACE_START

class EventSystem {
public:
    static void PushEvent(const Event& event);
    void pushEvent(const Event& event);

private:
    static EventSystem& Instance();

    std::queue<Event> eventQueue;
};

MACE_END

#endif //MACE_EVENTSYSTEM_H
