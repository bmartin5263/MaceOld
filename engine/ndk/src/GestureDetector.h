#ifndef GESTUREDETECTOR_H_
#define GESTUREDETECTOR_H_

#include <vector>

#include <android/sensor.h>
#include <android/log.h>
#include "JNIHelper.h"
#include "math/vecmath.h"
#include "Core.h"

MACE_NDK_START

const int DOUBLE_TAP_TIMEOUT = 300 * 1000000;
const int TAP_TIMEOUT = 180 * 1000000;
const int DOUBLE_TAP_SLOP = 100;
const int TOUCH_SLOP = 8;

enum {
    GESTURE_STATE_NONE = 0,
    GESTURE_STATE_START = 1,
    GESTURE_STATE_MOVE = 2,
    GESTURE_STATE_END = 4,
    GESTURE_STATE_ACTION = (GESTURE_STATE_START | GESTURE_STATE_END),
};
typedef int GestureState;

/******************************************************************
 * Base class of Gesture Detectors
 * GestureDetectors handles input events and detect gestures
 * Note that different detectors may detect gestures with an event at
 * same time. The caller needs to manage gesture priority accordingly
 *
 */
class GestureDetector {
public:
    virtual ~GestureDetector() = default;

    virtual void setConfiguration(AConfiguration *config);
    virtual GestureState detect(const AInputEvent *motionEvent) = 0;

protected:
    static int FindIndex(const AInputEvent *event, int id);

    float dpFactor{1.f};
};

MACE_NDK_END

#endif /* GESTUREDETECTOR_H_ */
