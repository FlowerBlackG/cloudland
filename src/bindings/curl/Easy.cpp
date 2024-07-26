// SPDX-License-Identifier: MulanPSL-2.0
/*
    libcurl c++ bindings

    libcurl easy interface

    https://curl.se/libcurl/c/
    https://curl.se/libcurl/c/libcurl-easy.html

    created on 2024.7.26 at Haidian, Beijing

*/


#include "./Easy.h"
#include "./init.h"


using namespace std;


namespace cloudland {
namespace bindings {
namespace curl {

Easy::Easy() {
    globalInit();

    this->handle = curl_easy_init();
}


Easy::~Easy() {

    if (this->handle) {
        curl_easy_cleanup(this->handle);
        this->handle = nullptr;
    }

}


CURL* Easy::getHandle() {
    return this->handle;
}


CURLcode Easy::setUrl(const string& url) {
    return curl_easy_setopt(this->handle, CURLOPT_URL, url.c_str());
}




}  // namespace curl
}  // namespace bindings
}  // namespace cloudland

