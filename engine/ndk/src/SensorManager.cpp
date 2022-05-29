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

#include <cmath>
#include <dlfcn.h>
#include "SensorManager.h"

MACE_NDK_START

void SensorManager::init(android_app* app) {
    ASensorManager* sensorManager = mace::ndk::AcquireASensorManagerInstance(app);
    accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    sensorEventQueue = ASensorManager_createEventQueue(sensorManager, app->looper, LOOPER_ID_USER, nullptr, nullptr);
}

void SensorManager::resume() {
    if (accelerometerSensor != nullptr) {
        ASensorEventQueue_enableSensor(sensorEventQueue, accelerometerSensor);
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate(sensorEventQueue, accelerometerSensor,(1000L / 60) * 1000);
    }
}

void SensorManager::suspend() {
    if (accelerometerSensor != nullptr) {
        ASensorEventQueue_disableSensor(sensorEventQueue, accelerometerSensor);
    }
}

void SensorManager::process() {
    if (accelerometerSensor != nullptr) {
        ASensorEvent event;
        while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0) {
            // Do Nothing
        }
    }
}

ASensorManager* AcquireASensorManagerInstance(android_app* app) {
    typedef ASensorManager *(*PF_GETINSTANCEFORPACKAGE)(const char *name);
    void* androidHandle = dlopen("libandroid.so", RTLD_NOW);
    auto getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE) dlsym(androidHandle, "ASensorManager_getInstanceForPackage");
    if (getInstanceForPackageFunc) {
        JNIEnv* env = nullptr;
        app->activity->vm->AttachCurrentThread(&env, nullptr);

        jclass android_content_Context = env->GetObjectClass(app->activity->clazz);
        jmethodID midGetPackageName = env->GetMethodID(android_content_Context,
                                                       "getPackageName",
                                                       "()Ljava/lang/String;");
        auto packageName= (jstring)env->CallObjectMethod(app->activity->clazz,
                                                            midGetPackageName);

        const char *nativePackageName = env->GetStringUTFChars(packageName, nullptr);
        ASensorManager* mgr = getInstanceForPackageFunc(nativePackageName);
        env->ReleaseStringUTFChars(packageName, nativePackageName);
        app->activity->vm->DetachCurrentThread();
        if (mgr) {
            dlclose(androidHandle);
            return mgr;
        }
    }

    typedef ASensorManager *(*PF_GETINSTANCE)();
    auto getInstanceFunc = (PF_GETINSTANCE)dlsym(androidHandle, "ASensorManager_getInstance");
    // by all means at this point, ASensorManager_getInstance should be available
    assert(getInstanceFunc);
    dlclose(androidHandle);

    return getInstanceFunc();
}

MACE_NDK_END
