// SPDX-License-Identifier: MulanPSL-2.0
/*
    Api tools for Alipan Driver

    created on 2024.7.27 at Haidian, Beijing

*/

#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <mutex>
#include "../../network/ApiResult.h"
#include "./FileTree.h"


namespace cloudland::fs::alipan { class MiniDB; }


namespace cloudland {
namespace fs {
namespace alipan {
namespace api {


extern std::shared_ptr<MiniDB> miniDB;


bool isLoggedIn();
bool isNotLoggedIn();

const std::string oauthAuthorizeUrl();
HttpStatusCode code2accessToken(const std::string& code);
int tryLogin(const std::string& code);

HttpStatusCode getDriveInfo(
    std::string* userid = nullptr,
    std::string* username = nullptr,
    std::string* userAvatar = nullptr,
    std::string* defaultDriveId = nullptr,
    std::string* resourceDriveId = nullptr,
    std::string* backupDriveId = nullptr
);



network::ApiResult<FileInfo> getFileInfoByPath(
    const std::string& driveId,
    const std::string& filePath
);


network::ApiResult<FileInfo> getFileInfo(
    const std::string& driveId,
    const std::string& fileId
);



network::ApiResult<
    std::vector<std::unique_ptr<FileInfo>>
> getFileList(
    const std::string& driveId,
    const std::string& parentFileId
);


struct DownloadUrlData {
    std::string url;
    std::string expiration;
    std::string method; 
};


network::ApiResult<DownloadUrlData> getDownloadUrl(
    const std::string& driveId,
    const std::string& fileId,
    time_t expireSec = 900
);



}  // namespace api
}  // namespace alipan
}  // namespace fs
}  // namespace cloudland


