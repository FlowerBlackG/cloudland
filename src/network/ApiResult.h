// SPDX-License-Identifier: MulanPSL-2.0
/*
    Api Result

    created on 2024.7.30 at Haidian, Beijing

*/


#pragma once

#include <cstdint>
#include <string>
#include <optional>


#include "./HttpStatusCode.h"

namespace cloudland {
namespace network {


template<typename T>
struct ApiResult {
    HttpStatusCode code;
    std::string msg;
    std::optional<T> data;
};



}
}
