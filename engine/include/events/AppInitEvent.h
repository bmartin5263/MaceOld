//
// Created by Brandon on 5/16/22.
//

#ifndef MACE_APPINITEVENT_H
#define MACE_APPINITEVENT_H

#include "EventBase.h"
#include "Core.h"

MACE_START

struct AppInitEvent {
    AppInitEvent() = default;
    explicit AppInitEvent(NativeWindow nativeWindow):
        nativeWindow(nativeWindow)
    {}

    NativeWindow nativeWindow;
};

MACE_END

#endif //MACE_APPINITEVENT_H
