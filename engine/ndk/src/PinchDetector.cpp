//
// Created by Brandon on 5/23/22.
//

#include "PinchDetector.h"

MACE_NDK_START

GestureState PinchDetector::detect(const AInputEvent* event) {
    GestureState ret = GESTURE_STATE_NONE;
    int32_t action = AMotionEvent_getAction(event);
    uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;
    this->event = event;

    int32_t count = AMotionEvent_getPointerCount(event);
    switch (flags) {
        case AMOTION_EVENT_ACTION_DOWN:
            pointers.push_back(AMotionEvent_getPointerId(event, 0));
            break;
        case AMOTION_EVENT_ACTION_POINTER_DOWN: {
            int32_t iIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                                                                                AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            pointers.push_back(AMotionEvent_getPointerId(event, iIndex));
            if (count == 2) {
                // Start new pinch
                ret = GESTURE_STATE_START;
            }
        } break;
        case AMOTION_EVENT_ACTION_UP:
            pointers.pop_back();
            break;
        case AMOTION_EVENT_ACTION_POINTER_UP: {
            int32_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                                                                               AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            int32_t released_pointer_id = AMotionEvent_getPointerId(event, index);

            std::vector<int32_t>::iterator it = pointers.begin();
            std::vector<int32_t>::iterator it_end = pointers.end();
            int32_t i = 0;
            for (; it != it_end; ++it, ++i) {
                if (*it == released_pointer_id) {
                    pointers.erase(it);
                    break;
                }
            }

            if (i <= 1) {
                // Reset pinch or drag
                if (count != 2) {
                    // Start new pinch
                    ret = GESTURE_STATE_START | GESTURE_STATE_END;
                }
            }
        } break;
        case AMOTION_EVENT_ACTION_MOVE:
            switch (count) {
                case 1:
                    break;
                default:
                    // Multi touch
                    ret = GESTURE_STATE_MOVE;
                    break;
            }
            break;
        case AMOTION_EVENT_ACTION_CANCEL:
            break;
    }

    return ret;
}

bool PinchDetector::getPointers(Vec2& v1, Vec2& v2) {
    if (pointers.size() < 2) return false;

    int32_t index = GestureDetector::FindIndex(event, pointers[0]);
    if (index == -1) return false;

    float x = AMotionEvent_getX(event, index);
    float y = AMotionEvent_getY(event, index);

    index = FindIndex(event, pointers[1]);
    if (index == -1) return false;

    float x2 = AMotionEvent_getX(event, index);
    float y2 = AMotionEvent_getY(event, index);

    v1 = Vec2(x, y);
    v2 = Vec2(x2, y2);

    return true;
}

MACE_NDK_END