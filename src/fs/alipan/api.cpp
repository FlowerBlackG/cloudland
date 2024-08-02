// SPDX-License-Identifier: MulanPSL-2.0
/*
    Api tools for Alipan Driver

    created on 2024.7.27 at Haidian, Beijing

*/

#include "./api.h"
#include "../../bindings/curl.h"
#include <nlohmann/json.hpp>
#include "../../utils/Log.h"
#include "../../utils/Time.h"

#include <chrono>
#include <ctime>

using namespace std;
using namespace cloudland::bindings;



namespace cloudland {
namespace fs {
namespace alipan {
namespace api {

string oauthAccessToken = "";
string oauthRefreshToken = "";
time_t oauthAccessTokenExpireTimeSec = 0;


static const string API_HOST = "https://openapi.alipan.com";

static const string APPID = "c196f5667dee4124a6e66e9f8f6a214b";
static const string OAUTH_CODE_CHALLENGE_CODE = "01234567890123456789012345678901234567890123456789";
static const string OAUTH_SCOPE = "user:base,file:all:read,file:all:write";
static const string OAUTH_REDIRECT_URI = "https://www.gardilily.com/cloudland/redirect-uri.php";


static void addAuthorizationHeader(curl::Easy& easy) {
    easy.setHeader("Authorization", "Bearer " + api::oauthAccessToken);
}


const string oauthAuthorizeUrl() {
    string res = API_HOST;
    res += "/oauth/authorize";
    
    res += "?client_id=";
    res += APPID;
    
    res += "&scope=";
    res += OAUTH_SCOPE;

    res += "&code_challenge_method=plain";
    res += "&response_type=code";

    res += "&code_challenge=";
    res += OAUTH_CODE_CHALLENGE_CODE;

    res += "&redirect_uri=";
    res += OAUTH_REDIRECT_URI;

    return res;
}


int64_t code2accessToken(const string& code) {
    curl::Easy easy;
    easy.post(API_HOST + "/oauth/access_token");
    nlohmann::json json;
    json["client_id"] = APPID;
    json["code"] = code;
    json["code_verifier"] = OAUTH_CODE_CHALLENGE_CODE;
    json["grant_type"] = "authorization_code";
    easy.setPostBody(json.dump())
        .setContentTypeJson()
        .execute();

    int64_t resCode = easy.responseCode();
    if (resCode != 200) {
        LOG_ERROR(easy.responseBody());
        LOG_ERROR("oauth code: ", code);
        return resCode;
    }

    
    nlohmann::json resJson = nlohmann::json::parse(easy.responseBody());
    if (!resJson.contains("access_token")) {
        LOG_ERROR("access_token not found in response json!");
        return 400;
    }

    if (!resJson.contains("expires_in")) {
        LOG_ERROR("expires_in not found in response json!");
        return 400;
    }

    oauthAccessToken = resJson["access_token"];
    oauthAccessTokenExpireTimeSec = utils::time::currentTimeSecs() + (time_t) resJson["expires_in"] - 30;

    return resCode;
}



int64_t getDriveInfo(
    string* userid,
    string* username,
    string* userAvatar,
    string* defaultDriveId,
    string* resourceDriveId,
    string* backupDriveId
) {
    string url = API_HOST + "/adrive/v1.0/user/getDriveInfo";
    curl::Easy easy;
    easy.post(url)
        .setContentTypeJson();
    
    addAuthorizationHeader(easy);

    easy.execute();


    if (easy.responseCode() != 200) {
        LOG_ERROR("response code is: ", easy.responseCode());
        return easy.responseCode();
    }

    LOG_WARN(easy.responseBody()) // todo

    auto json = nlohmann::json::parse(easy.responseBody());
    if (json.contains("user_id") && userid) {
        *userid = json["user_id"];
    }

    if (json.contains("name") && username) {
        *username = json["name"];
    }

    if (json.contains("default_drive_id") && defaultDriveId) {
        *defaultDriveId = json["default_drive_id"];
    }

    if (json.contains("resource_drive_id") && resourceDriveId) {
        *resourceDriveId = json["resource_drive_id"];
    }

    if (json.contains("backup_drive_id") && backupDriveId) {
        *backupDriveId = json["backup_drive_id"];
    }



    return easy.responseCode();
}


FileInfo FileInfo::createFrom(const nlohmann::json& json) {
    FileInfo info;
    info.load(json);
    return info;
}


int FileInfo::load(const nlohmann::json& json) {
    auto& info = *this;
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


network::ApiResult<FileInfo> getFileInfoByPath(
    const string& driveId,
    const string& filePath
) {
    network::ApiResult<FileInfo> result;

    string url = API_HOST + "/adrive/v1.0/openFile/get_by_path";
    curl::Easy easy;
    easy.post(url)
        .setContentTypeJson();
    addAuthorizationHeader(easy);

    nlohmann::json postBody;
    postBody["drive_id"] = driveId;
    postBody["file_path"] = filePath;

    easy.setPostBody(postBody).execute();

    result.code = easy.responseCode();

    if (result.code != 200) {
        result.msg = easy.responseBody();
        return result;
    }


    auto json = nlohmann::json::parse(easy.responseBody());
    FileInfo info = FileInfo::createFrom(json);
    if (info.error) {
        result.code = -1;
        result.msg = "response json error: " + info.errorMsg;
        return result;
    }

    result.data = info;

    return result;
}


network::ApiResult<FileInfo> getFileInfo(
    const string& driveId,
    const string& fileId
) {
network::ApiResult<FileInfo> result;

    string url = API_HOST + "/adrive/v1.0/openFile/get";
    curl::Easy easy;
    easy.post(url)
        .setContentTypeJson();
    addAuthorizationHeader(easy);

    nlohmann::json postBody;
    postBody["drive_id"] = driveId;
    postBody["file_id"] = fileId;

    easy.setPostBody(postBody).execute();

    result.code = easy.responseCode();

    if (result.code != 200) {
        result.msg = easy.responseBody();
        return result;
    }


    auto json = nlohmann::json::parse(easy.responseBody());
    FileInfo info = FileInfo::createFrom(json);
    if (info.error) {
        result.code = -1;
        result.msg = "response json error: " + info.errorMsg;
        return result;
    }

    result.data = info;

    return result;
}


network::ApiResult<
    vector<unique_ptr<FileInfo>>
> getFileList(
    const std::string& driveId,
    const std::string& parentFileId
) {
    network::ApiResult<vector<unique_ptr<FileInfo>>> result;
    nlohmann::json postBody;
    postBody["drive_id"] = driveId;
    postBody["parent_file_id"] = parentFileId;
    postBody["fields"] = "*";
    postBody["type"] = "all";

    curl::Easy easy;
    easy.post(API_HOST + "/adrive/v1.0/openFile/list")
        .setContentTypeJson()
        .setPostBody(postBody);
    addAuthorizationHeader(easy);
    easy.execute();

    result.code = easy.responseCode();
    if (result.code != 200) {
        result.msg = easy.responseBody();
        return result;
    }


    result.data = vector<unique_ptr<FileInfo>> {};
    auto& items = result.data.value();

    auto json = nlohmann::json::parse(easy.responseBody());

    for (auto& it : json["items"]) {
        auto p = make_unique<FileInfo>();
        p->load(it);
        if (p->error) {
            LOG_WARN(p->errorMsg)
            continue;
        }
        items.push_back(std::move(p));        
    }


    return result;
}



}  // namespace api
}  // namespace alipan
}  // namespace cloudland
}  // namespace fs


