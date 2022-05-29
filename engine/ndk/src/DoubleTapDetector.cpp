//
// Created by Brandon on 5/23/22.
//

#include "DoubleTapDetector.h"

MACE_NDK_START

GestureState DoubleTapDetector::detect(const AInputEvent* motionEvent) {
    if (AMotionEvent_getPointerCount(motionEvent) > 1) {
        // Only support single double tap
        return false;
    }

    bool tapDetected = tapDetector.detect(motionEvent);

    int32_t action = AMotionEvent_getAction(motionEvent);
    unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
    switch (flags) {
        case AMOTION_EVENT_ACTION_DOWN: {
            int64_t eventTime = AMotionEvent_getEventTime(motionEvent);
            if (eventTime - lastTapTime <= DOUBLE_TAP_TIMEOUT) {
                float x = AMotionEvent_getX(motionEvent, 0) - lastX;
                float y = AMotionEvent_getY(motionEvent, 0) - lastY;
                if (x * x + y * y < DOUBLE_TAP_SLOP * DOUBLE_TAP_SLOP * dpFactor) {
                    INFO("DoubleTapDetector: Doubletap detected");
                    return GESTURE_STATE_ACTION;
                }
            }
            break;
        }
        case AMOTION_EVENT_ACTION_UP:
            if (tapDetected) {
                lastTapTime = AMotionEvent_getEventTime(motionEvent);
                lastX = AMotionEvent_getX(motionEvent, 0);
                lastY = AMotionEvent_getY(motionEvent, 0);
            }
            break;
    }
    return GESTURE_STATE_NONE;
}

void DoubleTapDetector::setConfiguration(AConfiguration* config) {
    GestureDetector::setConfiguration(config);
    tapDetector.setConfiguration(config);
}

MACE_NDK_END
