// SPDX-License-Identifier: MulanPSL-2.0
/*
    libcurl c++ bindings

    libcurl easy interface
    https://curl.se/libcurl/c/
    https://curl.se/libcurl/c/libcurl-easy.html

    created on 2024.7.26 at Haidian, Beijing

*/


#pragma once

#include <curl/curl.h>
#include <string>


namespace cloudland {
namespace bindings {
namespace curl {

class Easy {
public:
    Easy();
    ~Easy();

    CURL* getHandle();

    CURLcode setUrl(const std::string& url);


protected:
    CURL* handle = nullptr;

};



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland

