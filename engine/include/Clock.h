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

#ifndef PERFMONITOR_H_
#define PERFMONITOR_H_

#include <errno.h>
#include <time.h>
#include "Core.h"

MACE_START

const int32_t SAMPLE_COUNT = 100;

class Clock {
public:
    Clock();
    virtual ~Clock() = default;

    bool update(float &fFPS);

    static double GetCurrentTime() {
        timeval timeOfDay = GetTimeOfDay();
        return timeOfDay.tv_sec + timeOfDay.tv_usec * 1.0 / 1000000.0;
    }

    static timeval GetTimeOfDay() {
        timeval timeOfDay{};
        gettimeofday(&timeOfDay, nullptr);
        return timeOfDay;
    }

private:
    double updateTick(double currentTick);

    double tickSum;
    double tickSamples[SAMPLE_COUNT];
    double lastTick;
    float currentFps;
    time_t lastTimeValue;
    int32_t tickIndex;
};

MACE_END

#endif /* PERFMONITOR_H_ */
