// SPDX-License-Identifier: MulanPSL-2.0
/*
    cloudland fs drivers.

    created on 2024.7.26 at Haidian, Beijing

*/


#include "./fs.h"
#include "../utils/Log.h"
#include "./FsDriverBase.h"
#include "./mirror/FsDriver.h"
#include "./alipan/FsDriver.h"

using namespace std;

namespace cloudland {
namespace fs {



FsDriverBase* getFSInstance(const string& name) {
    if (name == "mirror") {
        return mirror::FsDriver::getInstance();
    } else if (name == "onedrive") {

    } else if (name == "alipan") {
        return alipan::FsDriver::getInstance();
    } 


    return nullptr;
}



fuse_operations* getFuseOperations(const string& name) {
    auto dri = getFSInstance(name);
    return dri ? dri->getFuseOperations() : nullptr;
}



} // namespace fs
} // namespace cloudland


