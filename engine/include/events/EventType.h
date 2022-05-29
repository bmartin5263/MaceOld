//
// Created by Brandon on 5/16/22.
//

#ifndef MACE_EVENTTYPE_H
#define MACE_EVENTTYPE_H

MACE_START

enum class EventType {

    NONE = 0,

    /**
     * Triggered when the app starts up for the first time
     */
    APP_INIT = 1 << 1,

    /**
     * Triggered when the app is terminated (note, navigating away will not trigger a
     * APP_TERMINATE, it will trigger a APP_PAUSE)
     */
    APP_TERMINATE = 1 << 2,

    /**
     * Triggered when the app is navigated away from
     */
    APP_PAUSE = 1 << 3,

    /**
     * Triggered when the app is navigated back to
     */
    APP_RESUME = 1 << 4
};

MACE_END

#endif //MACE_EVENTTYPE_H
