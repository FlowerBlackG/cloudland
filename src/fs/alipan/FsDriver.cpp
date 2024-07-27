// SPDX-License-Identifier: MulanPSL-2.0
/*
    alipan by aliyun fs

    created on 2024.7.26 at Haidian, Beijing

*/



#include "./FsDriver.h"
#include "../../GlobalData.h"

#include <string>
#include <fstream>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include <nlohmann/json.hpp>

#include "../../bindings/curl.h"
#include "../../utils/Time.h"
#include "./api.h"


using namespace std;
using namespace cloudland::bindings;


namespace cloudland {
namespace fs {
namespace alipan {


#define OAUTH_NOTE_FILENAME "login.txt"
#define OAUTH_NOTE_FILEPATH "/" OAUTH_NOTE_FILENAME



static const string makeLoginNoteFileContent() {
    string res = "open link below in any browser, replace all of this file's content to the response uri, then close this file. \n\n";
    res += api::oauthAuthorizeUrl();
    res += "\n\n";
    return res;
}

CLOUDLAND_FS_PREPARE_CLASS_CPP()


static const string MINI_DB_FILEPATH = "/minidb.json";  // in data dir
FsDriver::FsDriver() {
    auto in = ifstream(globalData.dataDir + MINI_DB_FILEPATH, ios::binary);
    if (in.is_open()) {
        in >> this->miniDB;
    }
}


FsDriver::~FsDriver() {
    auto path = globalData.dataDir + MINI_DB_FILEPATH;
    auto out = ofstream(path, ios::binary);
    if (out.is_open()) {
        out << this->miniDB;
    } else {
        LOG_ERROR("[FATAL] Failed to dump minidb (alipan) to ", path, " !");
    }
}



int FsDriver::fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) {
    string strPath = path;
    string dataPath = globalData.dataDir + strPath;

    if (isNotLoggedIn()) {
        if (strPath == "/" || strPath == OAUTH_NOTE_FILEPATH) {

            st->st_gid = getgid();
            st->st_uid = getuid();
            st->st_blksize = 4096;
            st->st_size = 4096;
            st->st_blocks = 0;
            st->st_mode = S_IRWXU;

            if (strPath == "/") {
                st->st_mode |= S_IFDIR;
            } else if (strPath == OAUTH_NOTE_FILEPATH) {
                st->st_mode |= S_IFREG;
                st->st_size = makeLoginNoteFileContent().length();
            }
            
            return 0;
        } else {
            return -ENOENT;
        }
    }


    LOG_ERROR("not impl")
    return -ENOSYS;
}


int FsDriver::fsReadLink(const char* path, char* buf, size_t size) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsMkdir(const char* path, mode_t mode) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsUnlink(const char* path) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsRmdir(const char* path) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsSymlink(const char* from, const char* to) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsRename(const char* from, const char* to, unsigned int flags) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsLink(const char* from, const char* to) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsTruncate(const char* path, off_t size, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsOpen(const char* path, fuse_file_info* fi) {
    string strPath = path;
    if (isNotLoggedIn() && strPath == OAUTH_NOTE_FILEPATH) {
        return 0;
    }


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) {
    string strPath = path;
    if (isNotLoggedIn() && strPath == OAUTH_NOTE_FILEPATH) {
        const auto loginNote = makeLoginNoteFileContent();
        if ((uint64_t) (offset) >= loginNote.length()) {
            return 0;
        }

        const char* pContent = loginNote.data() + offset;
        char* pBuf = buf;
        while (size-- && *pContent) {
            *pBuf++ = *pContent++;
        }
        return (int) (pBuf - buf);
    }

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi) {
    string strPath = path;
    if (isNotLoggedIn() && strPath == OAUTH_NOTE_FILEPATH) {
        string content = buf;
        auto posOfCode = content.rfind("code=");
        if (posOfCode == string::npos) {
            return -ENOSYS;
        }

        auto oauthCode = content.substr(posOfCode + strlen("code="));

        // get access_token

        auto res = api::code2accessToken(oauthCode);
        LOG_WARN("token: ", api::oauthAccessToken);
        LOG_WARN("expir: ", api::oauthAccessTokenExpireTimeSec); // todo
        // todo
    }

    LOG_ERROR("Method not implemented.")
    return size;
    return -ENOSYS;
}


int FsDriver::fsStatfs(const char* path, struct statvfs* stbuf) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsSetXAttr(const char* path, const char* name, const char* value, size_t size, int flags) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsGetXAttr(const char* path, const char* name, char* value, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsListXAttr(const char* path, char* list, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsOpenDir(const char* path, fuse_file_info* fi) {
    string strPath = path;

    if (isNotLoggedIn()) {
        if (strPath != "/") {
            return -ENOSYS;
        } else {
            return 0;
        }
    }
    


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsReadDir(
    const char* path, 
    void* buf, 
    fuse_fill_dir_t filler, 
    off_t offset, 
    fuse_file_info* fi, 
    fuse_readdir_flags flags
) {
    string strPath = path;
    if (isNotLoggedIn()) {
        if (strPath != "/") {
            return -ENOSYS;
        } else {
            // show oauth note.

            struct stat st {0};
            st.st_mode = S_IRWXU;

            filler(buf, OAUTH_NOTE_FILENAME, &st, 0, FUSE_FILL_DIR_PLUS);

            return 0;
        }
    }

    
    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}



int FsDriver::fsReleaseDir(const char* path, fuse_file_info* fi) {

    return 0;
}


void* FsDriver::fsInit(fuse_conn_info* conn, fuse_config* cfg) {
    
    return nullptr;
}


void FsDriver::fsDestroy(void* privateData) {


    
}


int FsDriver::fsAccess(const char* path, int mask) {


    return 0;
}


int FsDriver::fsCreate(const char* path, mode_t mode, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


bool FsDriver::isLoggedIn() {
    if (!miniDB.contains(MiniDBKey::OAUTH_ACCESS_TOKEN)) {
        return false;
    }

    time_t expireTimeSec = miniDB[MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC];

    if (expireTimeSec < utils::time::currentTimeSecs()) {
        miniDB.erase(MiniDBKey::OAUTH_ACCESS_TOKEN);
        miniDB.erase(MiniDBKey::OAUTH_REFRESH_TOKEN);
        miniDB.erase(MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC);
        return false;  // access token expired.
    }
    

    return true;
}


bool FsDriver::isNotLoggedIn() {
    return !this->isLoggedIn();
}



// minidb keys' definitions

string MiniDBKey::OAUTH_ACCESS_TOKEN = "oauth-access-token";
string MiniDBKey::OAUTH_REFRESH_TOKEN = "oauth-refresh-token";
string MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC = "oauth-access-token-expire-time-sec";



}  // namespace alipan
}  // namespace cloudland
}  // namespace fs

