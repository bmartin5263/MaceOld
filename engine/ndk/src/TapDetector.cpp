//
// Created by Brandon on 5/23/22.
//

#include "TapDetector.h"

MACE_NDK_START

GestureState TapDetector::detect(const AInputEvent* motion_event) {
    if (AMotionEvent_getPointerCount(motion_event) > 1) {
        // Only support single touch
        return false;
    }

    int32_t action = AMotionEvent_getAction(motion_event);
    unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
    auto rawX = AMotionEvent_getRawX(motion_event, 0);
    auto rawY = AMotionEvent_getRawY(motion_event, 0);
    switch (flags) {
        case AMOTION_EVENT_ACTION_DOWN:
            downPointerId = AMotionEvent_getPointerId(motion_event, 0);
            downX = AMotionEvent_getX(motion_event, 0);
            downY = AMotionEvent_getY(motion_event, 0);
            DBUG("X==%f, Y==%f", rawX, rawY);
            break;
        case AMOTION_EVENT_ACTION_UP: {
            int64_t eventTime = AMotionEvent_getEventTime(motion_event);
            int64_t downTime = AMotionEvent_getDownTime(motion_event);
            if (eventTime - downTime <= TAP_TIMEOUT) {
                if (downPointerId == AMotionEvent_getPointerId(motion_event, 0)) {
                    float x = AMotionEvent_getX(motion_event, 0) - downX;
                    float y = AMotionEvent_getY(motion_event, 0) - downY;
                    if (x * x + y * y < TOUCH_SLOP * TOUCH_SLOP * dpFactor) {
                        INFO("TapDetector: Tap detected");
                        return GESTURE_STATE_ACTION;
                    }
                }
            }
            break;
        }
    }
    return GESTURE_STATE_NONE;
}

MACE_NDK_END