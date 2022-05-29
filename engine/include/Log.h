//
// Created by Brandon on 5/16/22.
//

#ifndef MACE_LOG_H
#define MACE_LOG_H

#include <string>
#include "Core.h"

MACE_START

class Log {
public:
    static void Debug(const char* name, const char* fmt, ...);
    static void Info(const char* name, const char* fmt, ...);
    static void Warn(const char* name, const char* fmt, ...);
    static void Error(const char* name, const char* fmt, ...);
};

MACE_END

#define DBUG(...) mace::Log::Debug(__FUNCTION__, __VA_ARGS__)
#define INFO(...) mace::Log::Info(__FUNCTION__, __VA_ARGS__)
#define WARN(...) mace::Log::Warn(__FUNCTION__, __VA_ARGS__)
#define ERROR(...) mace::Log::Error(__FUNCTION__, __VA_ARGS__)

#endif //MACE_LOG_H
