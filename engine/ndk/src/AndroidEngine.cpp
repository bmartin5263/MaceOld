//
// Created by Brandon on 5/14/22.
//

#ifdef __ANDROID__

#include "JNIHelper.h"
#include "CoreEngine.h"
#include "engine/EngineImpl.h"
#include "PlatformEngine.h"
#include "EventSystem.h"
#include "Log.h"
#include "Core.h"
#include "Util.h"

using namespace mace;
using namespace mace::ndk;

PlatformEngine::PlatformEngine(android_app *androidApp):
    app(androidApp)
{
    ERROR("AndroidEngine()");
}

void PlatformEngine::launch(EngineImpl *engine) {
    ERROR("AndroidEngine::launch()");
    doubleTapDetector.setConfiguration(this->app->config);
    dragDetector.setConfiguration(this->app->config);
    pinchDetector.setConfiguration(this->app->config);
//    mace::ndk::JNIHelper::Init(app->activity, HELPER_CLASS_NAME);
//
//    app->userData = this;
//    app->onAppCmd = Engine::OnAppCmd;
//    app->onInputEvent = Engine::OnInputEvent;
//    sensorManager.init(app);
}

void onAppCmd(android_app* app, int32_t cmd) {
    WARN("Processing Command: %s", getCommandString(cmd));
    Event event{};
    switch (cmd) {
        case APP_CMD_START:
        case APP_CMD_RESUME:
        case APP_CMD_INPUT_CHANGED:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != nullptr) {
                event.type = EventType::APP_INIT;
                event.appInitEvent = AppInitEvent(app->window);
                EventSystem::PushEvent(event);
            }
            break;
        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONTENT_RECT_CHANGED:
        case APP_CMD_WINDOW_REDRAW_NEEDED:
            break;
        case APP_CMD_GAINED_FOCUS:
            event.type = EventType::APP_RESUME;
            event.appResumeEvent = AppResumeEvent(app->window);
            break;
        case APP_CMD_CONFIG_CHANGED:
        case APP_CMD_LOW_MEMORY:
        case APP_CMD_PAUSE:
        case APP_CMD_LOST_FOCUS:
        case APP_CMD_TERM_WINDOW:
        case APP_CMD_STOP:
        case APP_CMD_SAVE_STATE:
        case APP_CMD_DESTROY:
        default:
            WARN("Unhandled Command %d", cmd);
    }
}

void CoreEngine::Init(android_app *app) {
    deviceEngine = new PlatformEngine(app);
}

#endif