//
// Created by Brandon on 5/23/22.
//

#ifndef MACE_DOUBLETAPDETECTOR_H
#define MACE_DOUBLETAPDETECTOR_H

#include "Core.h"
#include "TapDetector.h"

MACE_NDK_START

class DoubleTapDetector : public GestureDetector {
public:
    /**
     * Returns GESTURE_STATE_ACTION when a double-tap gesture is detected
     */
    GestureState detect(const AInputEvent *motionEvent) override;
    void setConfiguration(AConfiguration *config) override;

private:
    TapDetector tapDetector{};
    int64_t lastTapTime{};
    float lastX{};
    float lastY{};
};

MACE_NDK_END

#endif //MACE_DOUBLETAPDETECTOR_H
