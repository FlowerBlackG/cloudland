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
    

#ifndef NDEBUG
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
    curl_easy_setopt(this->handle, CURLOPT_POST, 1);
    return *this;
}


Easy& Easy::post(const string& url) {
    setUrl(url);
    post();
    return *this;
}



Easy& Easy::get() {
    curl_easy_setopt(this->handle, CURLOPT_HTTPGET, 1);
    return *this;
}


Easy& Easy::get(const string& url) {
    setUrl(url);
    get();
    return *this;
}


Easy& Easy::setPostBody(const string& data) {
    curl_easy_setopt(this->handle, CURLOPT_COPYPOSTFIELDS, data.data());
    return *this;
}


Easy& Easy::setHeader(const string& key, const string& value) {
    curl_easy_setopt(
        this->handle, 
        CURLOPT_HTTPHEADER, 
        (curl_slist*) this->headerHolder.emplace_back(key + ": " + value)
    );
    

    return *this;
}


void Easy::execute() {



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


string Easy::responseBody() {
    return this->recvBuf.str();
}



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland

