//
// Created by Brandon on 5/13/22.
//

#ifndef MACE_COREENGINE_H
#define MACE_COREENGINE_H

#ifdef __ANDROID__
#include <android_native_app_glue.h>
#endif

#include "Core.h"

MACE_START

class EngineImpl;
class EventSystem;
class PlatformEngine;
class CoreEngine {
public:
#ifdef __ANDROID__
    static void Init(android_app* app);
#endif

    static void Launch();
    static EventSystem& EventSystem();

    /*
    static void launch(Scene *scene);
    static void pushScene(Scene *scene);

    static double deltaTime();

    static FileSystem& fileSystem();
    static Graphics& graphics();
    static Keyboard& keyboard();
    static UI& ui();
    static ResourcesImpl& resources();
     */

private:
    static EngineImpl& Instance();
    static PlatformEngine* deviceEngine;

    CoreEngine() = delete;
    CoreEngine(CoreEngine& other) = delete;
    CoreEngine& operator=(CoreEngine& other) = delete;
    ~CoreEngine() = delete;
};

MACE_END

#endif //MACE_COREENGINE_H
