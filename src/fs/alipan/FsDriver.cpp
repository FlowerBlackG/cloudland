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
#include <map>
#include <chrono>
#include <ctime>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include <nlohmann/json.hpp>

#include "../../bindings/curl.h"
#include "../../utils/Time.h"
#include "./api.h"
#include "./MiniDB.h"


using namespace std;
using namespace cloudland::bindings;

// todo
static map<string, string> __fileid_map;


namespace cloudland {
namespace fs {
namespace alipan {


#define OAUTH_NOTE_FILENAME "login.txt"
#define OAUTH_NOTE_FILEPATH "/" OAUTH_NOTE_FILENAME

const string ALIPAN_RESOURCE_DRIVE_FOLDER_NAME = "resource";
const string ALIPAN_BACKUP_DRIVE_FOLDER_NAME = "backup";

const string ALIPAN_RESOURCE_DRIVE_FOLDER_PATH = "/" + ALIPAN_RESOURCE_DRIVE_FOLDER_NAME;
const string ALIPAN_BACKUP_DRIVE_FOLDER_PATH =  "/" + ALIPAN_BACKUP_DRIVE_FOLDER_NAME;



static const string makeLoginNoteFileContent() {
    string res = "open link below in any browser, replace all of this file's content to the response uri, then close this file. \n\n";
    res += api::oauthAuthorizeUrl();
    res += "\n\n";
    return res;
}

CLOUDLAND_FS_PREPARE_CLASS_CPP()


static const string MINI_DB_FILEPATH = "/minidb.json";  // in data dir
FsDriver::FsDriver() {

    this->miniDB = make_shared<MiniDB>();
    if (this->miniDB == nullptr) {
        LOG_ERROR("failed to alloc MiniDB for Fs Driver!");
        return;
    }

    auto in = ifstream(globalData.dataDir + MINI_DB_FILEPATH, ios::binary);
    if (in.is_open()) {
        this->miniDB->load(in);
    }

    if (isLoggedIn()) {
        api::oauthAccessToken = miniDB->db[MiniDBKey::OAUTH_ACCESS_TOKEN];
        api::oauthAccessTokenExpireTimeSec = miniDB->db[MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC];
    }


    // make FileService
    this->fileService = make_shared<FileService>(this->miniDB);
    if (this->fileService == nullptr) {
        LOG_ERROR("failed to prepare File Service!")
        return;
    }

    this->ready = true;
}


FsDriver::~FsDriver() {

}



static int guestGetAttr(const string& path, struct stat* st) {
    if (path == "/" || path == OAUTH_NOTE_FILEPATH) {

        st->st_gid = getgid();
        st->st_uid = getuid();
        st->st_blksize = 4096;
        st->st_size = 4096;
        st->st_blocks = 0;
        st->st_mode = S_IRWXU;

        if (path == "/") {
            st->st_mode |= S_IFDIR;
        } else if (path == OAUTH_NOTE_FILEPATH) {
            st->st_mode |= S_IFREG;
            st->st_size = makeLoginNoteFileContent().length();
        }
        
        return 0;
    } else {
        return -ENOENT;
    }
}


static int getAttrOfRoot(struct stat* st) {
    st->st_gid = getgid();
    st->st_uid = getuid();
    st->st_blksize = 4096;
    st->st_size = 4096;
    st->st_blocks = 0;
    st->st_mode = S_IRWXU | S_IFDIR;
    return 0;
}


int FsDriver::fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) {
    LOG_TEMPORARY("fsGetAttr | ", path)
    string strPath = path;

    if (isNotLoggedIn()) {
        return guestGetAttr(strPath, st);
    }

    if (
        strPath == "/" 
        || strPath == ALIPAN_BACKUP_DRIVE_FOLDER_PATH 
        || strPath == ALIPAN_RESOURCE_DRIVE_FOLDER_PATH
    ) {
        __fileid_map[ALIPAN_BACKUP_DRIVE_FOLDER_PATH] = "root"; // todo
        __fileid_map[ALIPAN_RESOURCE_DRIVE_FOLDER_PATH] = "root"; // todo
        return getAttrOfRoot(st);
    }


    string driveId = getDriveId(strPath, true);
    if (driveId.empty()) {
        return -ENOENT;
    }

    auto res = api::getFileInfoByPath(driveId, strPath);

    if (res.code != 200) {
        return -ENOENT;
    }

    // now, res.code is 200

    auto& fileInfo = res.data.value();

    st->st_gid = getgid();
    st->st_uid = getuid();
    st->st_blksize = 4096;
    st->st_blocks = (fileInfo.size + st->st_blksize + 1) / st->st_blksize;
    st->st_size = fileInfo.size;
    st->st_mode = S_IRWXU;
    st->st_mode |= fileInfo.isFile ? S_IFREG : S_IFDIR;

    st->st_ctim.tv_sec = fileInfo.getCreatedAtSecs();
    st->st_mtim.tv_sec = fileInfo.getUpdatedAtSecs();

    __fileid_map[path] = fileInfo.fileId; // todo

    return 0;
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
    LOG_TEMPORARY("fsOpen")
    string strPath = path;
    if (isNotLoggedIn() && strPath == OAUTH_NOTE_FILEPATH) {
        return 0;
    }


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


static int guestRead(const string& path, char* buf, size_t size, off_t offset) {
    if (path == OAUTH_NOTE_FILEPATH) {
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
    } else {
        return -ENOENT;
    }
}


int FsDriver::fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) {
    LOG_TEMPORARY("fsRead")
    string strPath = path;
    if (isNotLoggedIn()) {
        return guestRead(strPath, buf, size, offset);
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
        auto tryLoginResCode = tryLogin(oauthCode);
        if (tryLoginResCode) {
            return -ENOENT;
        } else {
            return size;
        }
    }


    LOG_ERROR("Method not implemented.")
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
    LOG_TEMPORARY("fsOpenDir | ", path)
    string strPath = path;

    if (isNotLoggedIn()) {
        if (strPath != "/") {
            return -ENOSYS;
        } else {
            return 0;
        }
    }
    

    return 0;
}


static int guestReadDir(
    const string& path, 
    void* buf, 
    fuse_fill_dir_t filler, 
    off_t offset, 
    fuse_file_info* fi, 
    fuse_readdir_flags flags
) {
    if (path != "/") {
        return -ENOENT;
    } else {
        // show oauth note.

        struct stat st {0};
        st.st_mode = S_IRWXU;

        filler(buf, OAUTH_NOTE_FILENAME, &st, 0, FUSE_FILL_DIR_PLUS);

        return 0;
    }
}


static int readDirOfRoot(  // for users logged-in
    const string& path, 
    void* buf,
    fuse_fill_dir_t filler,
    off_t offset,
    fuse_file_info* fi,
    fuse_readdir_flags flags
) {

    struct stat st {0};
    st.st_mode = S_IRWXU | S_IFDIR;

    filler(buf, ALIPAN_BACKUP_DRIVE_FOLDER_NAME.c_str(), &st, 0, FUSE_FILL_DIR_PLUS);
    filler(buf, ALIPAN_RESOURCE_DRIVE_FOLDER_NAME.c_str(), &st, 0, FUSE_FILL_DIR_PLUS);

    return 0;
}


int FsDriver::fsReadDir(
    const char* path, 
    void* buf, 
    fuse_fill_dir_t filler, 
    off_t offset, 
    fuse_file_info* fi, 
    fuse_readdir_flags flags
) {
    LOG_TEMPORARY("fsReadDir | ", path)
    string strPath = path;
    
    if (isNotLoggedIn()) {
        return guestReadDir(strPath, buf, filler, offset, fi, flags);
    } else if (strPath == "/") {
        return readDirOfRoot(strPath, buf, filler, offset, fi, flags);
    }

    string driveId = getDriveId(strPath, true);
    if (driveId.empty()) {
        return -ENOENT;
    }



    
    // todo
auto res =    api::getFileList(driveId, __fileid_map[path]); // todo

    for (auto& it : res.data.value()) {
        auto fileInfo = it.get();

        struct stat st {0};
        st.st_mode = S_IRWXU;

        filler(buf, fileInfo->name.data(), &st, 0, FUSE_FILL_DIR_PLUS);
    }
    
    return 0; // todo
}



int FsDriver::fsReleaseDir(const char* path, fuse_file_info* fi) {

    return 0;
}


void* FsDriver::fsInit(fuse_conn_info* conn, fuse_config* cfg) {
    
    this->fileService->startLoop();

    return nullptr;
}


void FsDriver::fsDestroy(void* privateData) {

    this->fileService->stopLoop(true);

    auto path = globalData.dataDir + MINI_DB_FILEPATH;
    auto out = ofstream(path, ios::binary);
    if (out.is_open()) {
        this->miniDB->dump(out);
    } else {
        LOG_ERROR("[FATAL] Failed to dump minidb (alipan) to ", path, " !");
    }
    
}


int FsDriver::fsAccess(const char* path, int mask) {


    return 0;
}


int FsDriver::fsCreate(const char* path, mode_t mode, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::tryLogin(const string& code) {
    // get access_token

    auto res = api::code2accessToken(code);
    if (res != 200) {
        return -1;
    }

    miniDB->db[MiniDBKey::OAUTH_ACCESS_TOKEN] = api::oauthAccessToken;
    miniDB->db[MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC] = api::oauthAccessTokenExpireTimeSec;

    string userid, defaultDriveId, backupDriveId, resourceDriveId;

    res = api::getDriveInfo(
        &userid, nullptr, nullptr, &defaultDriveId, &resourceDriveId, &backupDriveId
    );

    if (res != 200) {
        return -1;
    }

    miniDB->db[MiniDBKey::USERID] = userid;
    miniDB->db[MiniDBKey::DEFAULT_DRIVE_ID] = defaultDriveId;
    miniDB->db[MiniDBKey::BACKUP_DRIVE_ID] = backupDriveId;
    miniDB->db[MiniDBKey::RESOURCE_DRIVE_ID] = resourceDriveId;
    return 0;
}



string FsDriver::getDriveId(string& path, bool removePrefixFromPath) {
    string driveId;
    if (
        path.starts_with(ALIPAN_BACKUP_DRIVE_FOLDER_PATH + "/") || path == ALIPAN_BACKUP_DRIVE_FOLDER_PATH
    ) {

        driveId = miniDB->db[MiniDBKey::BACKUP_DRIVE_ID];

        if (removePrefixFromPath)
            path = path.substr(ALIPAN_BACKUP_DRIVE_FOLDER_PATH.length());
    
    } else if (

        path.starts_with(ALIPAN_RESOURCE_DRIVE_FOLDER_PATH + "/") || path == ALIPAN_RESOURCE_DRIVE_FOLDER_PATH
    
    ) {
        driveId = miniDB->db[MiniDBKey::RESOURCE_DRIVE_ID];
    
        if (removePrefixFromPath)
            path = path.substr(ALIPAN_RESOURCE_DRIVE_FOLDER_PATH.length());
    }

    return driveId;
}


bool FsDriver::isLoggedIn() {
    if (!miniDB->contains(MiniDBKey::OAUTH_ACCESS_TOKEN)) {
        return false;
    }

    time_t expireTimeSec = miniDB->getLong(MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC);

    if (expireTimeSec < utils::time::currentTimeSecs()) {
        miniDB->erase(MiniDBKey::OAUTH_ACCESS_TOKEN);
        miniDB->erase(MiniDBKey::OAUTH_REFRESH_TOKEN);
        miniDB->erase(MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC);
        return false;  // access token expired.
    }


    return true;
}


bool FsDriver::isNotLoggedIn() {
    return !this->isLoggedIn();
}




}  // namespace alipan
}  // namespace cloudland
}  // namespace fs

