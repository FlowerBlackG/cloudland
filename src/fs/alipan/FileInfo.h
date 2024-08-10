// SPDX-License-Identifier: MulanPSL-2.0
/*
    File Info structure

    created on 2024.8.7 at Haidian, Beijing

*/

#pragma once


#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <sys/stat.h>


namespace cloudland {
namespace fs {
namespace alipan {


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

    nlohmann::json& getJson();

    void to(struct stat*);

    int error = 0;
    std::string errorMsg;
    
    /**
     * should get this by getJson().
     */
    nlohmann::json json;
};




}  // namespace alipan
}  // namespace fs
}  // namespace cloudland

