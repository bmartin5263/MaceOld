//
// Created by Brandon on 5/23/22.
//

#ifndef MACE_TAPDETECTOR_H
#define MACE_TAPDETECTOR_H

#include "Core.h"
#include "GestureDetector.h"

MACE_NDK_START

class TapDetector : public GestureDetector {
public:
    /**
     * Returns GESTURE_STATE_ACTION when a tap gesture is detected
     */
    GestureState detect(const AInputEvent *motion_event) override;

private:
    int32_t downPointerId{};
    float downX{};
    float downY{};
};

MACE_NDK_END

#endif //MACE_TAPDETECTOR_H
