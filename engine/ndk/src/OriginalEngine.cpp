/*
 * Copyright 2018 The Android Open Source Project
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

#include "OriginalEngine.h"
#include "CoreEngine.h"
#include "Log.h"
#include "Util.h"

using namespace mace;

OriginalEngine::OriginalEngine(android_app* app):
    focused(false),
    app(app)
{
    doubleTapDetector.setConfiguration(this->app->config);
    dragDetector.setConfiguration(this->app->config);
    pinchDetector.setConfiguration(this->app->config);
    teapotRender.bind(&tapCamera);
    WARN("Constructed");
}

void OriginalEngine::loadResources() {
    INFO("Loading Resources");
    teapotRender.init(app->activity->assetManager);
}

void OriginalEngine::unloadResources() {
    INFO("Unloading Resources");
    teapotRender.destroy();
}

/**
 * Initialize an EGL context for the current display.
 */
int OriginalEngine::initWindow(android_app* app) {
    if (!glContext.isInitialized()) {
        glContext.init(app->window);

        loadResources();
        tapCamera.setFlip(1.f, -1.f, -1.f);
        tapCamera.setPinchTransformFactor(2.f, 2.f, 8.f);
    }
    else {
        glContext.resume(app->window);
    }
    teapotRender.updateViewport();

    return 0;
}

void OriginalEngine::updateFPS() {
    float fps;
    if (engineClock.update(fps)) {
        JNIEnv *jni;
        app->activity->vm->AttachCurrentThread(&jni, nullptr);

        // Default class retrieval
        jclass clazz = jni->GetObjectClass(app->activity->clazz);
        jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");
        jni->CallVoidMethod(app->activity->clazz, methodID, fps);

        app->activity->vm->DetachCurrentThread();
    }
}

void OriginalEngine::drawFrame() {
    updateFPS();
    teapotRender.update(static_cast<float>(Clock::GetCurrentTime()));

    // Just fill the screen with a color.
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    teapotRender.render();

    glContext.swap();
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void OriginalEngine::termWindow() {
    glContext.suspend();
}

void OriginalEngine::handleDrag(AInputEvent* event) {
    using namespace mace::ndk;
    GestureState dragState = dragDetector.detect(event);
    if (dragState & GESTURE_STATE_START) {
        // Otherwise, start dragging
        Vec2 v;
        dragDetector.getPointer(v);
        toGlCoordinates(v);
        tapCamera.beginDrag(v);
    } else if (dragState & GESTURE_STATE_MOVE) {
        Vec2 v;
        dragDetector.getPointer(v);
        toGlCoordinates(v);
        tapCamera.drag(v);
    } else if (dragState & GESTURE_STATE_END) {
        tapCamera.endDrag();
    }
}

void OriginalEngine::handlePinch(AInputEvent* event) {
    using namespace mace::ndk;
    GestureState pinchState = pinchDetector.detect(event);
    if (pinchState & mace::ndk::GESTURE_STATE_START) {
        // Start new pinch
        Vec2 v1, v2;
        pinchDetector.getPointers(v1, v2);
        toGlCoordinates(v1, v2);
        tapCamera.beginPinch(v1, v2);
    } else if (pinchState & mace::ndk::GESTURE_STATE_MOVE) {
        // Multi touch
        // Start new pinch
        Vec2 v1, v2;
        pinchDetector.getPointers(v1, v2);
        toGlCoordinates(v1, v2);
        tapCamera.pinch(v1, v2);
    }
}

/**
 * Process the next input event.
 */
int32_t OriginalEngine::OnInputEvent(android_app* app, AInputEvent* event) {
    auto* engine = static_cast<OriginalEngine*>(app->userData);
    return engine->handleInputEvent(app, event);
}

/**
 * Process the next main command.
 */
void OriginalEngine::OnAppCmd(android_app* app, int32_t cmd) {
    auto* engine = static_cast<OriginalEngine*>(app->userData);
    engine->handleCmd(app, cmd);
}

bool OriginalEngine::hasFocus() const {
    return focused;
}

void OriginalEngine::toGlCoordinates(Vec2 &vec) {
    auto w = static_cast<float>(glContext.getScreenWidth());
    auto h = static_cast<float>(glContext.getScreenHeight());
    vec = Vec2(2.0f, 2.0f) * vec /
          Vec2(w,h) -
          Vec2(1.f, 1.f);
}

void OriginalEngine::toGlCoordinates(Vec2 &v1, Vec2 &v2) {
    toGlCoordinates(v1);
    toGlCoordinates(v2);
}

//void OriginalEngine::showUI() {
//    JNIEnv *jni;
//    app->activity->vm->AttachCurrentThread(&jni, nullptr);
//
//    // Default class retrieval
//    jclass clazz = jni->GetObjectClass(app->activity->clazz);
//    jmethodID methodID = jni->GetMethodID(clazz, "showUI", "()V");
//    jni->CallVoidMethod(app->activity->clazz, methodID);
//
//    app->activity->vm->DetachCurrentThread();
//}

const char* HELPER_CLASS_NAME = "dev/bdon/helper/NDKHelper";

void OriginalEngine::run() {
    mace::ndk::JNIHelper::Init(app->activity, HELPER_CLASS_NAME);

    app->userData = this;
    app->onAppCmd = OriginalEngine::OnAppCmd;
    app->onInputEvent = OriginalEngine::OnInputEvent;
    sensorManager.init(app);

#ifdef USE_NDK_PROFILER
    monstartup("libSpacegameActivity.so");
#endif

    while (update()) {
        draw();
    }

    ERROR("Shutdown");
}

bool OriginalEngine::update() {
    int id;
    int events;
    android_poll_source *source;

    id = ALooper_pollAll(
        hasFocus() ? 0 : -1,
        nullptr,
        &events,
        (void **) &source
    );
    while(id >= 0) {
        // 1 = Command (source != null)
        // 2 = Input Event (source != null)
        // 3 = Clock tick?

        if (id > 3) {
            WARN("Polled: %d", id);
        }

        // Process this event.
        if (source != nullptr) {
            if (id > 3) {
                WARN("Source Process: %d", id);
            }
            source->process(app, source);
        }

        if (id == LOOPER_ID_USER) {
            sensorManager.process();
        }

        // Check if we are exiting.
        if (app->destroyRequested != 0) {
            termWindow();
            return false;
        }
        id = ALooper_pollAll(
            hasFocus() ? 0 : -1,
            nullptr,
            &events,
            (void **) &source
        );
    }
    return true;
}

void OriginalEngine::draw() {
    if (hasFocus()) {
        // Drawing is throttled to the screen update rate, so there
        // is no need to do timing here.
        drawFrame();
    }
}

void OriginalEngine::handleCmd(android_app *app, int32_t cmd) {
    WARN("Processing Command: %s", mace::ndk::getCommandString(cmd));
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            ASSERT(app->window != nullptr, "Received nullptr for window")
            // The window is being shown, get it ready.
            initWindow(app);
//            drawFrame();
            break;
        case APP_CMD_GAINED_FOCUS:
            sensorManager.resume();
            focused = true;
            break;
        case APP_CMD_LOST_FOCUS:
            sensorManager.suspend();
            focused = false;
            drawFrame();
            break;
        case APP_CMD_TERM_WINDOW:
            termWindow();
            break;
        default:
            break;
    }
}

int32_t OriginalEngine::handleInputEvent(android_app *app, AInputEvent *event) {
    using namespace mace::ndk;
    auto* engine = static_cast<OriginalEngine*>(app->userData);
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        // Double tap detector has a priority over other detectors
        GestureState doubleTapState = engine->doubleTapDetector.detect(event);
        if (doubleTapState == GESTURE_STATE_ACTION) {
            // Detect double tap
            engine->tapCamera.reset(true);
        } else {
            engine->handleDrag(event);
            engine->handlePinch(event);
        }
        return 1;
    }
    return 0;
}