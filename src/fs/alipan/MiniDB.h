// SPDX-License-Identifier: MulanPSL-2.0
/*
    minidb for alipan fs driver

    created on 2024.8.2 at Haidian, Beijing

*/


#pragma once

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>


namespace cloudland {
namespace fs {
namespace alipan {
    

struct MiniDBKey {
protected:
    MiniDBKey() = delete;

public:
    static std::string OAUTH_ACCESS_TOKEN;
    static std::string OAUTH_REFRESH_TOKEN;
    static std::string OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC;

    static std::string USERID;
    static std::string DEFAULT_DRIVE_ID;
    static std::string RESOURCE_DRIVE_ID;
    static std::string BACKUP_DRIVE_ID;


    static std::string FILE_SERVICE_DATA;

};


class MiniDB {
public:

    void load(std::istream&);
    void dump(std::ostream&);


    bool contains(const std::string&);
    void erase(const std::string& key);

    int64_t getLong(const std::string& key);
    int64_t getLong(const std::string& key, int64_t fallback);
    std::string getString(const std::string& key);
    std::string getString(const std::string& key, const std::string& fallback);
    

    nlohmann::json db;

};


}  // namespace alipan
}  // namespace cloudland
}  // namespace fs

