// SPDX-License-Identifier: MulanPSL-2.0
/*
    libcurl c++ bindings

    libcurl slist structure

    created on 2024.7.27 at Haidian, Beijing

*/

#include "./SList.h"
#include "../../utils/Log.h"

using namespace std;

namespace cloudland {
namespace bindings {
namespace curl {

SList::SList() {

}


SList::SList(const string& str) {
    this->append(str);
}


SList::~SList() {
    curl_slist_free_all(this->curlSList);
}


SList& SList::append(const string& str) {
    this->curlSList = curl_slist_append(this->curlSList, str.c_str());

    return *this;
}


SList:: operator curl_slist* () {
    return this->curlSList;
}



}  // namespace curl
}  // namespace bindings
}  // namespace cloudland



