#include "GestureDetector.h"

MACE_NDK_START

void GestureDetector::setConfiguration(AConfiguration* config) {
    dpFactor = 160.f / static_cast<float>(AConfiguration_getDensity(config));
}

int GestureDetector::FindIndex(const AInputEvent* event, int id) {
    auto count = AMotionEvent_getPointerCount(event);
    for (auto i = 0; i < count; ++i) {
        if (id == AMotionEvent_getPointerId(event, i)) {
            return i;
        }
    }
    return -1;
}

MACE_NDK_END
