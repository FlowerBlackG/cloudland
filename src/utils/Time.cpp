// SPDX-License-Identifier: MulanPSL-2.0

/*
 * time utils
 * 
 * created on 2024.07.27 at Haidian, Beijing
 */


#include "./Time.h"
#include <ctime>

using namespace std;

namespace cloudland {
namespace utils {
namespace time {

time_t currentTimeSecs() {
    timespec curr;
    clock_gettime(CLOCK_MONOTONIC, &curr);
    return curr.tv_sec;
}



}  // namespace time
}  // namespace utils
}  // namespace cloudland

