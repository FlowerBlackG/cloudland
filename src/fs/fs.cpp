// SPDX-License-Identifier: MulanPSL-2.0
/*
    cloudland fs drivers.

    created on 2024.7.26 at Haidian, Beijing

*/


#include "./fs.h"
#include "../utils/Log.h"
#include "./Base.h"
#include "./Mirror/Mirror.h"
#include "./Alipan/Alipan.h"

using namespace std;

namespace cloudland {
namespace fs {



Base* getFSInstance(const string& name) {
    if (name == "mirror") {
        return Mirror::getInstance();
    } else if (name == "onedrive") {

    } else if (name == "alipan") {
        return Alipan::getInstance();
    } 

#ifndef NDEBUG
    else if (name == "base") {
        LOG_WARN("'base' should only be used for debug purpose.")
        return Base::getInstance();
    }
#endif


    return nullptr;
}



fuse_operations* getFuseOperations(const string& name) {
    auto dri = getFSInstance(name);
    return dri ? dri->getFuseOperations() : nullptr;
}



} // namespace fs
} // namespace cloudland


