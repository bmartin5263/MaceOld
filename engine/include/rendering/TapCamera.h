/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _TAP_CAMERA_H
#define _TAP_CAMERA_H

#include <Core.h>
#include "c++/v1/vector"
#include "c++/v1/string"
#include "math/vecmath.h"

MACE_START

/******************************************************************
 * Camera control helper class with a tap gesture
 * This class is mainly used for 3D space camera control in samples.
 *
 */
class TapCamera {
public:
    TapCamera();

    void beginDrag(const Vec2& vec);
    void endDrag();
    void drag(const Vec2& vec);
    void update();
    void update(const double time);

    Mat4& getRotationMatrix();
    Mat4& getTransformMatrix();

    void beginPinch(const Vec2& v1, const Vec2& v2);
    void endPinch();
    void pinch(const Vec2& v1, const Vec2& v2);

    void setFlip(const float x, const float y, const float z) {
        vecFlip = Vec2(x, y);
        flipZ = z;
    }

    void setPinchTransformFactor(const float x, const float y, const float z) {
        vecPinchTransformFactor = Vec3(x, y, z);
    }

    void reset(const bool bAnimate);
private:
    // Trackball
    Vec2 trackballCenter;
    float ballRadius;
    Quaternion quatBallNow;
    Quaternion quatBallDown;
    Vec2 vecBallNow;
    Vec2 vecBallDown;
    Quaternion quatBallRot;

    bool dragging;
    bool pinching;

    // Pinch related info
    Vec2 vecPinchStart;
    Vec2 vecPinchStartCenter;
    float pinchStartDistanceSq;

    // Camera shift
    Vec3 vecOffset;
    Vec3 vecOffsetNow;

    // Camera Rotation
    float cameraRotation;
    float cameraRotationStart;
    float cameraRotationNow;

    // Momentum support
    bool momentum;
    double momentumTimeStamp;
    Vec2 vecDragDelta;
    Vec2 vecLastInput;
    Vec3 vecOffsetLast;
    Vec3 vecOffsetDelta;
    float momemtumSteps;

    Vec2 vecFlip;
    float flipZ;

    Mat4 matRotation;
    Mat4 matTransform;

    Vec3 vecPinchTransformFactor;

    Vec3 pointOnSphere(Vec2& point);
    void ballUpdate();
    void initParameters();
};

MACE_END

#endif
