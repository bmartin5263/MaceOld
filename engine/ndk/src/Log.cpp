//
// Created by Brandon on 5/28/22.
//

#include "Log.h"
#include <android/log.h>

void Log::print(const std::string &s) {
    __android_log_print(ANDROID_LOG_INFO, "Mace-NDK", "%s", s.c_str());
}