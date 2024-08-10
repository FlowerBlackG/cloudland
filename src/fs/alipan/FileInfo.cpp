// SPDX-License-Identifier: MulanPSL-2.0
/*
    File Info structure

    created on 2024.8.7 at Haidian, Beijing

*/


#include "./FileInfo.h"

#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;


namespace cloudland {
namespace fs {
namespace alipan {



FileInfo FileInfo::createFrom(const nlohmann::json& json) {
    FileInfo info;
    info.load(json);
    info.json = json;
    return info;
}


int FileInfo::load(const nlohmann::json& json) {
    auto& info = *this;
    info.json = json;
    info.error = 0;
    info.errorMsg.clear();
    info.isFolder = info.isFile = false;
    
    struct {
        string jsonKey;
        unsigned long memberOffset;
        bool optional = false;
    } batchProcessKeys[] = {
        { "drive_id", offsetof(FileInfo, driveId) },
        { "file_id", offsetof(FileInfo, fileId) },
        { "parent_file_id", offsetof(FileInfo, parentFileId) },
        { "name", offsetof(FileInfo, name) },
        { "file_extension", offsetof(FileInfo, fileExtension), true },
        { "content_hash", offsetof(FileInfo, contentHash), true },
        { "category", offsetof(FileInfo, category), true },
        { "thumbnail", offsetof(FileInfo, thumbnail), true },
        { "url", offsetof(FileInfo, url), true },
        { "created_at", offsetof(FileInfo, createdAt) },
        { "updated_at", offsetof(FileInfo, updatedAt) },
        { "id_path", offsetof(FileInfo, idPath), true },
        { "name_path", offsetof(FileInfo, namePath), true },
    };

    for (auto& it : batchProcessKeys) {
        if (!json.contains(it.jsonKey) || json[it.jsonKey].is_null()) {
            if (it.optional) 
                continue;

            info.error = 1;
            info.errorMsg = "key " + it.jsonKey + " not found.";
            return info.error;
        }

        *(string*) (intptr_t(&info) + it.memberOffset) = json[it.jsonKey];
    }


    if (json["type"] == "file") {
        info.isFile = true;
    } else if (json["type"] == "folder") {
        info.isFolder = true;
    } else {
        info.error = 2;
        info.errorMsg = "file type " + ((string) json["type"]) + " unknown.";
        return info.error;
    }


    info.size = info.isFile ? size_t(json["size"]) : 4096;


    return info.error;
}



static time_t iso8601ToSecs(const string& iso) {
    istringstream in {iso};
    chrono::sys_time<chrono::seconds> tp;
    in >> chrono::parse("%Y-%m-%dT%H:%M:%S.", tp);

    return tp.time_since_epoch().count();
}


time_t FileInfo::getUpdatedAtSecs() {
    return iso8601ToSecs(this->updatedAt);
}


time_t FileInfo::getCreatedAtSecs() {
    return iso8601ToSecs(this->createdAt);
}


nlohmann::json& FileInfo::getJson() {
    return this->json;
}


void FileInfo::to(struct stat* st) {
    st->st_gid = getgid();
    st->st_uid = getuid();
    st->st_blksize = 4096;
    st->st_size = this->size;
    st->st_blocks = (size + st->st_blksize + 1) / st->st_blksize;

    st->st_mode = S_IRWXU;
    st->st_mode |= isFile ? S_IFREG : S_IFDIR;

    st->st_ctim.tv_sec = getCreatedAtSecs();
    st->st_mtim.tv_sec = getUpdatedAtSecs();
    st->st_atim.tv_sec = st->st_mtim.tv_sec;

}


}  // namespace alipan
}  // namespace fs
}  // namespace cloudland


