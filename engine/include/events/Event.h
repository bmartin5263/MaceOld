//
// Created by Brandon on 5/15/22.
//

#ifndef MACE_EVENT_H
#define MACE_EVENT_H

#include "Core.h"
#include "Events.h"

MACE_START

struct Event {
//    Event(EventType type) :
//        type{type}
//    {}

    EventType type;
    union {
        AppInitEvent appInitEvent;
        AppResumeEvent appResumeEvent;
    };
};

MACE_END

#endif //MACE_EVENT_H
