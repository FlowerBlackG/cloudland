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


enum class RequestMethod {
    POST, GET, HEAD, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH
};


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
    Easy& setRequestMethod(const RequestMethod&);
    Easy& setPostBody(const std::string& data);

    Easy& setContentTypeJson();
    Easy& setHeader(const std::string& key, const std::string& value);


    void execute();


    int64_t responseCode();
    
    std::string responseBody();

public:
    std::stringstream recvBuf;


protected:
    CURL* handle = nullptr;

    SList customHeaders;
    bool postBodyFilled = false;
    RequestMethod requestMethod = RequestMethod::GET;
    
    int64_t httpResponseCode = 0;
};



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland

