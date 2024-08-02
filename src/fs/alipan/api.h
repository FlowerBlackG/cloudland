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
#include "../../network/ApiResult.h"


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




struct FileInfo {
    std::string driveId, fileId, parentFileId, name, fileExtension, contentHash,
        category, thumbnail, url, createdAt, updatedAt, idPath, namePath;

    size_t size = 0;

    bool isFolder = false;
    bool isFile = false;


    static FileInfo createFrom(const nlohmann::json& json);
    int load(const nlohmann::json& json);
    time_t getUpdatedAtSecs();
    time_t getCreatedAtSecs();

    int error = 0;
    std::string errorMsg;
};


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



}  // namespace api
}  // namespace alipan
}  // namespace cloudland
}  // namespace fs


