//
// Created by Brandon on 5/23/22.
//

#include "DragDetector.h"

MACE_NDK_START

GestureState DragDetector::detect(const AInputEvent* event) {
    GestureState ret = GESTURE_STATE_NONE;
    auto action = AMotionEvent_getAction(event);
    auto index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    auto flags = action & AMOTION_EVENT_ACTION_MASK;
    this->event = event;

    auto count = static_cast<int32_t>(AMotionEvent_getPointerCount(event));
    switch (flags) {
        case AMOTION_EVENT_ACTION_DOWN:
            pointers.push_back(AMotionEvent_getPointerId(event, 0));
            ret = GESTURE_STATE_START;
            break;
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
            pointers.push_back(AMotionEvent_getPointerId(event, index));
            break;
        case AMOTION_EVENT_ACTION_UP:
            pointers.pop_back();
            ret = GESTURE_STATE_END;
            break;
        case AMOTION_EVENT_ACTION_POINTER_UP: {
            int32_t released_pointer_id = AMotionEvent_getPointerId(event, index);

            auto it = pointers.begin();
            auto it_end = pointers.end();
            int32_t i = 0;
            for (; it != it_end; ++it, ++i) {
                if (*it == released_pointer_id) {
                    pointers.erase(it);
                    break;
                }
            }

            if (i <= 1) {
                // Reset pinch or drag
                if (count == 2) {
                    ret = GESTURE_STATE_START;
                }
            }
            break;
        }
        case AMOTION_EVENT_ACTION_MOVE:
            if (count == 1) {
                ret = GESTURE_STATE_MOVE;
            }
            break;
        case AMOTION_EVENT_ACTION_CANCEL:
            break;
    }

    return ret;
}

bool DragDetector::getPointer(Vec2& v) {
    if (pointers.empty()) {
        return false;
    }

    int32_t iIndex = GestureDetector::FindIndex(event, pointers[0]);
    if (iIndex == -1) {
        return false;
    }

    float x = AMotionEvent_getX(event, iIndex);
    float y = AMotionEvent_getY(event, iIndex);

    v = Vec2(x, y);

    return true;
}

MACE_NDK_END
