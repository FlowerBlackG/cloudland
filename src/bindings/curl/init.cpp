// SPDX-License-Identifier: MulanPSL-2.0
/*
    libcurl c++ bindings

    created on 2024.7.26 at Haidian, Beijing

*/

#include "./init.h"
#include "../../utils/Log.h"

#include <curl/curl.h>

namespace cloudland {
namespace bindings {
namespace curl {

static bool initialized = false;
static bool cleaned = false;

void globalInit() {
    if (initialized) {
        return;
    }

    curl_global_init(CURL_GLOBAL_ALL);
    initialized = true;
}


static void globalCleanup() {

    if (cleaned || !initialized) {
        return;
    }

    curl_global_cleanup();
    cleaned = true;
}


static struct CloudlandBindingsCurlInitAutoDestructor {

    ~CloudlandBindingsCurlInitAutoDestructor() {
        globalCleanup();
    }

} autoDestructor;



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland
