//
// Created by Brandon on 5/21/22.
//

#ifndef MACE_CORE_H
#define MACE_CORE_H

#ifdef __ANDROID__
#include <android/log.h>
#define ASSERT(cond, ...) if (!(cond)) {__android_log_assert(#cond, __FUNCTION__, __VA_ARGS__);}
#endif

#define MACE_START namespace mace {
#define MACE_END }

#define MACE_NDK_START namespace mace { namespace ndk {
#define MACE_NDK_END }}

#define del(x) delete x; x = nullptr;

#endif //MACE_CORE_H
