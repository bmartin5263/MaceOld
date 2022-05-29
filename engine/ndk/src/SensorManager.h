#ifndef SENSORMANAGER_H_
#define SENSORMANAGER_H_

#include <android/sensor.h>
#include "JNIHelper.h"
#include "Core.h"

MACE_NDK_START

enum ORIENTATION {
    ORIENTATION_UNKNOWN = -1,
    ORIENTATION_PORTRAIT = 0,
    ORIENTATION_LANDSCAPE = 1,
    ORIENTATION_REVERSE_PORTRAIT = 2,
    ORIENTATION_REVERSE_LANDSCAPE = 3,
};

/*
 * Helper to handle sensor inputs such as accelerometer.
 * The helper also check for screen rotation
 */
class SensorManager {
public:
    void init(android_app* app);
    void resume();
    void suspend();
    void process();

private:
    const ASensor* accelerometerSensor{nullptr};
    ASensorEventQueue* sensorEventQueue{nullptr};
};

ASensorManager *AcquireASensorManagerInstance(android_app *app);

MACE_NDK_END
#endif /* SENSORMANAGER_H_ */
