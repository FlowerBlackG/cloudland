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
    easy.setPostBody(json.dump());
    easy.setHeader("Content-Type", "application/json");
    easy.execute();

    int64_t resCode = easy.responseCode();
    if (resCode != 200) {
        LOG_ERROR(easy.responseBody());
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




}  // namespace api
}  // namespace alipan
}  // namespace cloudland
}  // namespace fs


