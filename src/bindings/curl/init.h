// SPDX-License-Identifier: MulanPSL-2.0
/*
    libcurl c++ bindings

    created on 2024.7.26 at Haidian, Beijing

*/

#pragma once


namespace cloudland {
namespace bindings {
namespace curl {


/**
 * It calls curl_global_cleanup(). You should never (and must not) 
 * mind curl_global_cleanup().
 * 
 * Can be called multiple times.
 * 
 * Once called and returned, following calls would be ignored.
 * 
 * If called when first call is still executing (not returned),
 * it occurs undefined behaviours.
 */
void globalInit();



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland
