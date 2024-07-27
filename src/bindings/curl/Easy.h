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
#include <sstream>
#include <vector>
#include "./SList.h"


namespace cloudland {
namespace bindings {
namespace curl {

class Easy {
public:
    Easy();
    ~Easy();

    CURL* getHandle();

    Easy& setUrl(const std::string& url);
    Easy& post();
    Easy& post(const std::string& url);
    Easy& get();
    Easy& get(const std::string& url);
    Easy& setPostBody(const std::string& data);


    Easy& setHeader(const std::string& key, const std::string& value);


    void execute();


    int64_t responseCode();
    
    std::string responseBody();

public:
    std::stringstream recvBuf;


protected:
    CURL* handle = nullptr;

    /**
     * When setting header, libcurl internals doesn't copies the content.
     * So we have to keep it until the Easy object is used and retired.
     */
    std::vector<SList> headerHolder;
    
    int64_t httpResponseCode = 0;
};



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland

