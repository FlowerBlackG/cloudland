// SPDX-License-Identifier: MulanPSL-2.0

/*
 * logging util
 * 
 * created on 2023.12.26 at Anting, Jiading, Shanghai
 */

#include "Log.h"

#include <ctime>
#include <semaphore>

using namespace std;

namespace cloudland {
namespace log {

binary_semaphore logLock {1};
    
inline static void printLineInfo(const char* filename, const char* funcname, int line) {
    clog << "[" 
        << filename 
        << " " 
        << funcname
        << " "
        << line 
        << "]";
}

inline static void printCurrentTime() {
    time_t now = time(nullptr);
    tm* currTm = localtime(&now);
    char timeStr[32] = "";
    strftime(
        timeStr, 
        sizeof(timeStr) / sizeof(char), 
        "[%Y-%m-%d %H:%M:%S]", 
        currTm
    );
    clog << timeStr;
}

void printInfo(const char* filename, const char* funcname, int line) {
    printCurrentTime();
    
#ifndef NDEBUG
    clog << " ";
    printLineInfo(filename, funcname, line);
#endif

    clog << " ";
}

} // namespace log
} // namespace cloudland
