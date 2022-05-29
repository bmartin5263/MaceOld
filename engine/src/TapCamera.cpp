#include <fstream>
#include "rendering/TapCamera.h"

MACE_START

const float TRANSFORM_FACTOR = 15.f;
const float TRANSFORM_FACTORZ = 10.f;

const float MOMENTUM_FACTOR_DECREASE = 0.85f;
const float MOMENTUM_FACTOR_DECREASE_SHIFT = 0.9f;
const float MOMENTUM_FACTOR = 0.8f;
const float MOMENTUM_FACTOR_THRESHOLD = 0.001f;

TapCamera::TapCamera()
    : ballRadius(0.75f),
      dragging(false),
      pinching(false),
      pinchStartDistanceSq(0.f),
      cameraRotation(0.f),
      cameraRotationStart(0.f),
      cameraRotationNow(0.f),
      momentum(false),
      momemtumSteps(0.f),
      flipZ(0.f) {
    // Init offset
    initParameters();

    vecFlip = Vec2(1.f, -1.f);
    flipZ = -1.f;
    vecPinchTransformFactor = Vec3(1.f, 1.f, 1.f);

    trackballCenter = Vec2(0, 0);
    vecBallNow = Vec2(0, 0);
    vecBallDown = Vec2(0, 0);

    vecPinchStart = Vec2(0, 0);
    vecPinchStartCenter = Vec2(0, 0);

    vecFlip = Vec2(0, 0);
}

void TapCamera::initParameters() {
    // Init parameters
    vecOffset = Vec3();
    vecOffsetNow = Vec3();

    quatBallRot = Quaternion();
    quatBallNow = Quaternion();
    quatBallNow.ToMatrix(matRotation);
    cameraRotation = 0.f;

    vecDragDelta = Vec2();
    vecOffsetDelta = Vec3();

    momentum = false;
}

//----------------------------------------------------------
//  Dtor
//----------------------------------------------------------

void TapCamera::update() {
    if (momentum) {
        float momentumSteps = momemtumSteps;

        // Momentum rotation
        Vec2 v = vecDragDelta;
        beginDrag(Vec2());  // NOTE:This call reset _VDragDelta
        drag(v * vecFlip);

        // Momentum shift
        vecOffset += vecOffsetDelta;

        ballUpdate();
        endDrag();

        // Decrease deltas
        vecDragDelta = v * MOMENTUM_FACTOR_DECREASE;
        vecOffsetDelta = vecOffsetDelta * MOMENTUM_FACTOR_DECREASE_SHIFT;

        // Count steps
        momemtumSteps = momentumSteps * MOMENTUM_FACTOR_DECREASE;
        if (momemtumSteps < MOMENTUM_FACTOR_THRESHOLD) {
            momentum = false;
        }
    } else {
        vecDragDelta *= MOMENTUM_FACTOR;
        vecOffsetDelta = vecOffsetDelta * MOMENTUM_FACTOR;
        ballUpdate();
    }

    Vec3 vec = vecOffset + vecOffsetNow;
    Vec3 vec_tmp(TRANSFORM_FACTOR, -TRANSFORM_FACTOR, TRANSFORM_FACTORZ);

    vec *= vec_tmp * vecPinchTransformFactor;

    matTransform = Mat4::Translation(vec);
}

void TapCamera::update(const double time) {
    if (momentum) {
        const float MOMENTAM_UNIT = 0.0166f;
        // Activate every 16.6msec
        if (time - momentumTimeStamp >= MOMENTAM_UNIT) {
            float momenttum_steps = momemtumSteps;

            // Momentum rotation
            Vec2 v = vecDragDelta;
            beginDrag(Vec2());  // NOTE:This call reset _VDragDelta
            drag(v * vecFlip);

            // Momentum shift
            vecOffset += vecOffsetDelta;

            ballUpdate();
            endDrag();

            // Decrease deltas
            vecDragDelta = v * MOMENTUM_FACTOR_DECREASE;
            vecOffsetDelta = vecOffsetDelta * MOMENTUM_FACTOR_DECREASE_SHIFT;

            // Count steps
            momemtumSteps = momenttum_steps * MOMENTUM_FACTOR_DECREASE;
            if (momemtumSteps < MOMENTUM_FACTOR_THRESHOLD) {
                momentum = false;
            }
            momentumTimeStamp = time;
        }
    } else {
        vecDragDelta *= MOMENTUM_FACTOR;
        vecOffsetDelta = vecOffsetDelta * MOMENTUM_FACTOR;
        ballUpdate();
        momentumTimeStamp = time;
    }

    Vec3 vec = vecOffset + vecOffsetNow;
    Vec3 vec_tmp(TRANSFORM_FACTOR, -TRANSFORM_FACTOR, TRANSFORM_FACTORZ);

    vec *= vec_tmp * vecPinchTransformFactor;

    matTransform = Mat4::Translation(vec);
}
Mat4& TapCamera::getRotationMatrix() {
    return matRotation;
}

Mat4& TapCamera::getTransformMatrix() {
    return matTransform;
}

void TapCamera::reset(const bool bAnimate) {
    initParameters();
    update();
}

//----------------------------------------------------------
// Drag control
//----------------------------------------------------------
void TapCamera::beginDrag(const Vec2& v) {
    if (dragging) endDrag();

    if (pinching) endPinch();

    Vec2 vec = v * vecFlip;
    vecBallNow = vec;
    vecBallDown = vecBallNow;

    dragging = true;
    momentum = false;
    vecLastInput = vec;
    vecDragDelta = Vec2();
}

void TapCamera::endDrag() {
    quatBallDown = quatBallNow;
    quatBallRot = Quaternion();

    dragging = false;
    momentum = true;
    momemtumSteps = 1.0f;
}

void TapCamera::drag(const Vec2& v) {
    if (!dragging) return;

    Vec2 vec = v * vecFlip;
    vecBallNow = vec;

    vecDragDelta = vecDragDelta * MOMENTUM_FACTOR + (vec - vecLastInput);
    vecLastInput = vec;
}

//----------------------------------------------------------
// Pinch controll
//----------------------------------------------------------
void TapCamera::beginPinch(const Vec2& v1, const Vec2& v2) {
    if (dragging) endDrag();

    if (pinching) endPinch();

    beginDrag(Vec2());

    vecPinchStartCenter = (v1 + v2) / 2.f;

    Vec2 vec = v1 - v2;
    float x_diff;
    float y_diff;
    vec.Value(x_diff, y_diff);

    pinchStartDistanceSq = x_diff * x_diff + y_diff * y_diff;
    cameraRotationStart = atan2f(y_diff, x_diff);
    cameraRotationNow = 0;

    pinching = true;
    momentum = false;

    // Init momentum factors
    vecOffsetDelta = Vec3();
}

void TapCamera::endPinch() {
    pinching = false;
    momentum = true;
    momemtumSteps = 1.f;
    vecOffset += vecOffsetNow;
    cameraRotation += cameraRotationNow;
    vecOffsetNow = Vec3();

    cameraRotationNow = 0;

    endDrag();
}

void TapCamera::pinch(const Vec2& v1, const Vec2& v2) {
    if (!pinching) return;

    // Update momentum factor
    vecOffsetLast = vecOffsetNow;

    float x_diff, y_diff;
    Vec2 vec = v1 - v2;
    vec.Value(x_diff, y_diff);

    float fDistanceSQ = x_diff * x_diff + y_diff * y_diff;

    float f = pinchStartDistanceSq / fDistanceSQ;
    if (f < 1.f)
        f = -1.f / f + 1.0f;
    else
        f = f - 1.f;
    if (std::isnan(f)) f = 0.f;

    vec = (v1 + v2) / 2.f - vecPinchStartCenter;
    vecOffsetNow = Vec3(vec, flipZ * f);

    // Update momentum factor
    vecOffsetDelta = vecOffsetDelta * MOMENTUM_FACTOR +
                     (vecOffsetNow - vecOffsetLast);

    //
    // Update ration quaternion
    float fRotation = atan2f(y_diff, x_diff);
    cameraRotationNow = fRotation - cameraRotationStart;

    // Trackball rotation
    quatBallRot = Quaternion(0.f, 0.f, sinf(-cameraRotationNow * 0.5f),
                             cosf(-cameraRotationNow * 0.5f));
}

//----------------------------------------------------------
// Trackball controll
//----------------------------------------------------------
void TapCamera::ballUpdate() {
    if (dragging) {
        Vec3 vec_from = pointOnSphere(vecBallDown);
        Vec3 vec_to = pointOnSphere(vecBallNow);

        Vec3 vec = vec_from.Cross(vec_to);
        float w = vec_from.Dot(vec_to);

        Quaternion qDrag = Quaternion(vec, w);
        qDrag = qDrag * quatBallDown;
        quatBallNow = quatBallRot * qDrag;
    }
    quatBallNow.ToMatrix(matRotation);
}

Vec3 TapCamera::pointOnSphere(Vec2& point) {
    Vec3 ball_mouse;
    float mag;
    Vec2 vec = (point - trackballCenter) / ballRadius;
    mag = vec.Dot(vec);
    if (mag > 1.f) {
        float scale = 1.f / sqrtf(mag);
        vec *= scale;
        ball_mouse = Vec3(vec, 0.f);
    } else {
        ball_mouse = Vec3(vec, sqrtf(1.f - mag));
    }
    return ball_mouse;
}

MACE_END
