//
// Created by Brandon on 5/23/22.
//

#ifndef MACE_PINCHDETECTOR_H
#define MACE_PINCHDETECTOR_H

#include "Core.h"
#include "GestureDetector.h"

MACE_NDK_START

/******************************************************************
 * Double gesture detector
 * Returns pinch gesture state when a pinch gesture is detected
 * The class handles multiple touches more than 2
 * When the finger 1,2,3 are tapped and then finger 1 is released,
 * the detector start new pinch gesture with finger 2 & 3.
 */
class PinchDetector : public GestureDetector {
public:
    GestureState detect(const AInputEvent *event) override;
    bool getPointers(Vec2 &v1, Vec2 &v2);

private:
    const AInputEvent *event{nullptr};
    std::vector<int32_t> pointers{};
};

MACE_NDK_END

#endif //MACE_PINCHDETECTOR_H
