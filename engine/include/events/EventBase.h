//
// Created by Brandon on 5/16/22.
//

#ifndef MACE_EVENTBASE_H
#define MACE_EVENTBASE_H

#ifdef __ANDROID__
#include "android_native_app_glue.h"
using NativeWindow = ANativeWindow*;
#else
using NativeWindow = void*
#endif

#endif //MACE_EVENTBASE_H
