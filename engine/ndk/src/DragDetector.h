//
// Created by Brandon on 5/23/22.
//

#ifndef MACE_DRAGDETECTOR_H
#define MACE_DRAGDETECTOR_H

#include "Core.h"
#include "GestureDetector.h"

MACE_NDK_START

/******************************************************************
 * drag gesture detector
 * Returns drag gesture state when a drag-tap gesture is detected
 *
 */
class DragDetector : public GestureDetector {
public:
    GestureState detect(const AInputEvent *event) override;
    bool getPointer(Vec2 &v);

private:
    const AInputEvent *event{nullptr};
    std::vector<int32_t> pointers{};
};

MACE_NDK_END

#endif //MACE_DRAGDETECTOR_H
