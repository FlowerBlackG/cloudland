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
#include "./MiniDB.h"


using namespace std;
using namespace cloudland::bindings;



namespace cloudland {
namespace fs {
namespace alipan {
namespace api {


static const string API_HOST = "https://openapi.alipan.com";

static const string APPID = "c196f5667dee4124a6e66e9f8f6a214b";
static const string OAUTH_CODE_CHALLENGE_CODE = "01234567890123456789012345678901234567890123456789";
static const string OAUTH_SCOPE = "user:base,file:all:read,file:all:write";
static const string OAUTH_REDIRECT_URI = "https://www.gardilily.com/cloudland/redirect-uri.php";


shared_ptr<MiniDB> miniDB;


static int addAuthorizationHeader(curl::Easy& easy) {
    if (miniDB == nullptr) {
        return -1;
    }

    easy.setHeader("Authorization", "Bearer " + miniDB->getString(MiniDBKey::OAUTH_ACCESS_TOKEN));
    return 0;
}


bool isLoggedIn() {
    if (miniDB == nullptr) {
        return false;
    }

    time_t expireTimeSec = miniDB->getLong(MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC, 0);

    if (expireTimeSec < utils::time::currentTimeSecs()) {
        miniDB->erase(MiniDBKey::OAUTH_ACCESS_TOKEN);
        miniDB->erase(MiniDBKey::OAUTH_REFRESH_TOKEN);
        miniDB->erase(MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC);
        return false;  // access token expired.
    }


    return true;
}


bool isNotLoggedIn() {
    return !isLoggedIn();
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


HttpStatusCode code2accessToken(const string& code) {
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

    auto resCode = easy.httpStatusCode();
    if (resCode != HttpStatusCode::OK) {
        LOG_ERROR(easy.responseBody());
        LOG_ERROR("oauth code: ", code);
        return resCode;
    }

    
    nlohmann::json resJson = nlohmann::json::parse(easy.responseBody());
    if (!resJson.contains("access_token")) {
        LOG_ERROR("access_token not found in response json!");
        return HttpStatusCode::BAD_REQUEST;
    }

    if (!resJson.contains("expires_in")) {
        LOG_ERROR("expires_in not found in response json!");
        return HttpStatusCode::BAD_REQUEST;
    }

    if (miniDB) {
        miniDB->useDB([&] (nlohmann::json& db) {
            
            db[MiniDBKey::OAUTH_ACCESS_TOKEN] = resJson["access_token"];

            auto expireTimeSecs = utils::time::currentTimeSecs() + (time_t) resJson["expires_in"] - 30;
            db[MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC] = expireTimeSecs;
        });
    }

    return resCode;
}


int tryLogin(const string& code) {
    // get access_token

    auto res = api::code2accessToken(code);
    if (res != HttpStatusCode::OK) {
        return -1;
    }


    string userid, defaultDriveId, backupDriveId, resourceDriveId;

    res = api::getDriveInfo(
        &userid, nullptr, nullptr, &defaultDriveId, &resourceDriveId, &backupDriveId
    );

    if (res != HttpStatusCode::OK) {
        return -1;
    }

    miniDB->useDB([&] (nlohmann::json& db) {
        db[MiniDBKey::USERID] = userid;
        db[MiniDBKey::DEFAULT_DRIVE_ID] = defaultDriveId;
        db[MiniDBKey::BACKUP_DRIVE_ID] = backupDriveId;
        db[MiniDBKey::RESOURCE_DRIVE_ID] = resourceDriveId;
    });

    return 0;
}



HttpStatusCode getDriveInfo(
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
        return easy.httpStatusCode();
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



    return easy.httpStatusCode();
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

    result.code = easy.httpStatusCode();

    if (result.code != HttpStatusCode::OK) {
        result.msg = easy.responseBody();
        return result;
    }


    auto json = nlohmann::json::parse(easy.responseBody());
    FileInfo info = FileInfo::createFrom(json);
    if (info.error) {
        result.code = HttpStatusCode::BAD_REQUEST;
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

    result.code = easy.httpStatusCode();

    if (result.code != HttpStatusCode::OK) {
        result.msg = easy.responseBody();
        return result;
    }


    auto json = nlohmann::json::parse(easy.responseBody());
    FileInfo info = FileInfo::createFrom(json);
    if (info.error) {
        result.code = HttpStatusCode::BAD_REQUEST;
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

    result.code = easy.httpStatusCode();
    if (result.code != HttpStatusCode::OK) {
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
}  // namespace fs
}  // namespace cloudland


