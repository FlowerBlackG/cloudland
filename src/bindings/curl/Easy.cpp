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
#include "../../utils/Log.h"


using namespace std;


namespace cloudland {
namespace bindings {
namespace curl {


static size_t curlWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    if (nmemb == 0) {
        return 0;
    }

    auto pEasy = (Easy*) userdata;

    auto oldWritePos = pEasy->recvBuf.tellp();
    pEasy->recvBuf.write(ptr, nmemb);
    return (size_t) (pEasy->recvBuf.tellp() - oldWritePos);
}


Easy::Easy() {
    globalInit();


    this->handle = curl_easy_init();
    

#ifdef CLOUDLAND_LOG_LIBCURL
    curl_easy_setopt(this->handle, CURLOPT_VERBOSE, 1);
#endif

    curl_easy_setopt(this->handle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(this->handle, CURLOPT_WRITEFUNCTION, curlWriteCallback);
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


Easy& Easy::setUrl(const string& url) {
    curl_easy_setopt(this->handle, CURLOPT_URL, url.c_str());
    return *this;
}


Easy& Easy::post() {
    this->setRequestMethod(RequestMethod::POST);
    return *this;
}


Easy& Easy::post(const string& url) {
    setUrl(url);
    post();
    return *this;
}



Easy& Easy::get() {
    this->setRequestMethod(RequestMethod::GET);
    return *this;
}


Easy& Easy::get(const string& url) {
    setUrl(url);
    get();
    return *this;
}


Easy& Easy::setRequestMethod(const RequestMethod& method) {
    if (method != RequestMethod::GET && method != RequestMethod::POST) {
        LOG_ERROR("not supported.")
        return *this;
    }


    if (method == RequestMethod::GET) {
        curl_easy_setopt(this->handle, CURLOPT_HTTPGET, 1);
    } else if (method == RequestMethod::POST) {
        curl_easy_setopt(this->handle, CURLOPT_POST, 1);
    }

    this->requestMethod = method;


    return *this;
}


Easy& Easy::setPostBody(const string& data) {
    return setPostBody(data.data());
}


Easy& Easy::setPostBody(const char* data) {
    curl_easy_setopt(this->handle, CURLOPT_COPYPOSTFIELDS, data);
    postBodyFilled = true;
    return *this;
}



Easy& Easy::setPostBody(const nlohmann::json& json) {
    return setPostBody(json.dump());
}


Easy& Easy::setContentTypeJson() {
    return this->setHeader("Content-Type", "application/json");
}


Easy& Easy::setHeader(const string& key, const string& value) {
    this->customHeaders.append(key + ": " + value);
    return *this;
}


void Easy::execute() {
    if (this->customHeaders.isNotEmpty()) {
        curl_easy_setopt(this->handle, CURLOPT_HTTPHEADER, this->customHeaders.raw());
    }

    if (this->requestMethod == RequestMethod::POST && !this->postBodyFilled) {
        this->setPostBody("");
        // if we don't do this, libcurl will block itself when calling 'perform'.
    }

    auto code = curl_easy_perform(this->handle);
    if (code != CURLE_OK) {
        this->httpResponseCode = 400;
    } else {
        curl_easy_getinfo(this->handle, CURLINFO_RESPONSE_CODE, &this->httpResponseCode);
    }
}


int64_t Easy::responseCode() {
    return this->httpResponseCode;
}


HttpStatusCode Easy::httpStatusCode() {
    return static_cast<HttpStatusCode>(this->httpResponseCode);
}


string Easy::responseBody() {
    return this->recvBuf.str();
}


nlohmann::json Easy::responseBodyJson() {
    return nlohmann::json::parse(this->responseBody());
}



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland

