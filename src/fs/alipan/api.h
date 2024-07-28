// SPDX-License-Identifier: MulanPSL-2.0
/*
    Api tools for Alipan Driver

    created on 2024.7.27 at Haidian, Beijing

*/

#pragma once

#include <string>
#include <cstdint>


namespace cloudland {
namespace fs {
namespace alipan {
namespace api {

extern std::string oauthAccessToken;
extern std::string oauthRefreshToken;
extern time_t oauthAccessTokenExpireTimeSec;



const std::string oauthAuthorizeUrl();
int64_t code2accessToken(const std::string& code);

int64_t getDriveInfo(
    std::string* userid = nullptr,
    std::string* username = nullptr,
    std::string* userAvatar = nullptr,
    std::string* defaultDriveId = nullptr,
    std::string* resourceDriveId = nullptr,
    std::string* backupDriveId = nullptr
);



}  // namespace api
}  // namespace alipan
}  // namespace cloudland
}  // namespace fs


