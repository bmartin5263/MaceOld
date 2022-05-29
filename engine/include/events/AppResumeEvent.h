//
// Created by Brandon on 5/16/22.
//

#ifndef MACE_APPRESUMEEVENT_H
#define MACE_APPRESUMEEVENT_H

#include "EventBase.h"
#include "Core.h"

MACE_START

struct AppResumeEvent {
    AppResumeEvent() = default;
    explicit AppResumeEvent(NativeWindow nativeWindow):
        nativeWindow(nativeWindow)
    {}
    NativeWindow nativeWindow;
};

MACE_END

#endif //MACE_APPRESUMEEVENT_H
