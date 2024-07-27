// SPDX-License-Identifier: MulanPSL-2.0
/*
    libcurl c++ bindings

    libcurl slist structure

    created on 2024.7.27 at Haidian, Beijing

*/

#pragma once

#include <curl/curl.h>
#include <string>


namespace cloudland {
namespace bindings {
namespace curl {


class SList {
public:
    SList();
    SList(const std::string&);
    ~SList();

    SList& append(const std::string&);


    operator curl_slist* ();

protected:
    curl_slist* curlSList = nullptr;


};



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland



