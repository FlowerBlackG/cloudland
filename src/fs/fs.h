// SPDX-License-Identifier: MulanPSL-2.0
/*
    cloudland fs drivers.

    created on 2024.7.26 at Haidian, Beijing

*/

#pragma once

#include <string>
#include <fuse3/fuse.h>


namespace cloudland {
namespace fs {

class Base;


Base* getFSInstance(const std::string& name);
fuse_operations* getFuseOperations(const std::string& name);





} // namespace fs
} // namespace cloudland

