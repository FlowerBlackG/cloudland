// SPDX-License-Identifier: MulanPSL-2.0

/*
 * logging util
 *
 * 
 * format:
 * [main.cpp 32] xxx
 * 
 * created on 2023.12.26 at Anting, Jiading, Shanghai
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <semaphore>
#include "./ConsoleColorPad.h"


namespace cloudland {
namespace log {

extern std::binary_semaphore logLock;

#define CLOUDLAND_LOG_MUTEX_LOCK() cloudland::log::logLock.acquire();
#define CLOUDLAND_LOG_MUTEX_UNLOCK() cloudland::log::logLock.release();

void printInfo(const char* filename, int line);

inline void printContent() {
    std::clog << std::endl;
}

template <typename T, typename... Args>
inline void printContent(T value, Args... args) {
    std::clog << value;
    printContent(args...);
}

inline void resetColor() {
    ConsoleColorPad::setClogColor();
}

inline void setColorInfo() {
    ConsoleColorPad::setClogColor(255, 255, 255);
}

inline void setColorWarn() {
    ConsoleColorPad::setClogColor(255, 192, 64);
}

inline void setColorError() {
    ConsoleColorPad::setClogColor(255, 64, 64);
}


} // namespace log
} // namespace cloudland


#ifndef __FILE_NAME__
    #define __FILE_NAME__ ""
#endif

#define LOG_PLAIN(...) \
    { \
        cloudland::log::printInfo( \
            __FILE_NAME__, __LINE__ \
        ); \
        cloudland::log::printContent(__VA_ARGS__); \
    }

#define LOG_INFO(...) \
    { \
        CLOUDLAND_LOG_MUTEX_LOCK() \
        cloudland::log::setColorInfo(); \
        LOG_PLAIN(__VA_ARGS__); \
        cloudland::log::resetColor(); \
        CLOUDLAND_LOG_MUTEX_UNLOCK() \
    }

#define LOG_ERROR(...) \
    { \
        CLOUDLAND_LOG_MUTEX_LOCK() \
        cloudland::log::setColorError(); \
        LOG_PLAIN(__VA_ARGS__); \
        cloudland::log::resetColor(); \
        CLOUDLAND_LOG_MUTEX_UNLOCK() \
    }

#define LOG_WARN(...) \
    { \
        CLOUDLAND_LOG_MUTEX_LOCK() \
        cloudland::log::setColorWarn(); \
        LOG_PLAIN(__VA_ARGS__); \
        cloudland::log::resetColor(); \
        CLOUDLAND_LOG_MUTEX_UNLOCK() \
    }

#if 1
    #define LOG_TEMPORARY(...) { \
        CLOUDLAND_LOG_MUTEX_LOCK() \
        ConsoleColorPad::setClogColor(128, 128, 255); \
        LOG_PLAIN(__VA_ARGS__); \
        cloudland::log::resetColor(); \
        CLOUDLAND_LOG_MUTEX_UNLOCK() \
    }
#endif

#if 1
    #define TODO(...) { \
        CLOUDLAND_LOG_MUTEX_LOCK() \
        ConsoleColorPad::setClogColor(255, 192, 192); \
        LOG_PLAIN("TODO: ", __VA_ARGS__); \
        cloudland::log::resetColor(); \
        CLOUDLAND_LOG_MUTEX_UNLOCK() \
        exit(-1); \
    }
#endif

