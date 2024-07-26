// SPDX-License-Identifier: MulanPSL-2.0
/*
    alipan by aliyun fs

    created on 2024.7.26 at Haidian, Beijing

*/



#include "./Alipan.h"
#include "../../GlobalData.h"

#include <string>

#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


using namespace std;

namespace cloudland {
namespace fs {


#define OAUTH_NOTE_FILENAME "login.txt"
#define OAUTH_NOTE_FILEPATH "/" OAUTH_NOTE_FILENAME

#define OAUTH_NOTE_OAUTH_URL "https://openapi.alipan.com/oauth/authorize?client_id=c196f5667dee4124a6e66e9f8f6a214b&redirect_uri=https://www.gardilily.com/cloudland/redirect-uri.php&scope=user:base,file:all:read,file:all:write"

#define OAUTH_NOTE_CONTENT "open link below in any browser, replace all of this file's content to the response uri, then close this file. \n\n" OAUTH_NOTE_OAUTH_URL "\n\n"

static const string OAUTH_NOTE_CONTENT_STR(OAUTH_NOTE_CONTENT);


CLOUDLAND_FS_PREPARE_CLASS_CPP(Alipan)





int Alipan::fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) {
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
                st->st_size = OAUTH_NOTE_CONTENT_STR.length();
            }
            
            return 0;
        } else {
            return -ENOENT;
        }
    }


    LOG_ERROR("not impl")
    return -ENOSYS;
}


int Alipan::fsReadLink(const char* path, char* buf, size_t size) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsMkdir(const char* path, mode_t mode) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsUnlink(const char* path) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsRmdir(const char* path) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsSymlink(const char* from, const char* to) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsRename(const char* from, const char* to, unsigned int flags) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsLink(const char* from, const char* to) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsTruncate(const char* path, off_t size, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsOpen(const char* path, fuse_file_info* fi) {
    string strPath = path;
    if (isNotLoggedIn() && strPath == OAUTH_NOTE_FILEPATH) {
        return 0;
    }


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) {
    string strPath = path;
    if (isNotLoggedIn() && strPath == OAUTH_NOTE_FILEPATH) {
        if (offset >= OAUTH_NOTE_CONTENT_STR.length()) {
            return 0;
        }

        const char* pContent = OAUTH_NOTE_CONTENT_STR.data() + offset;
        char* pBuf = buf;
        while (size-- && *pContent) {
            *pBuf++ = *pContent++;
        }
        return (int) (pBuf - buf);
    }

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsStatfs(const char* path, struct statvfs* stbuf) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsSetXAttr(const char* path, const char* name, const char* value, size_t size, int flags) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsGetXAttr(const char* path, const char* name, char* value, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsListXAttr(const char* path, char* list, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Alipan::fsOpenDir(const char* path, fuse_file_info* fi) {
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


int Alipan::fsReadDir(
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



int Alipan::fsReleaseDir(const char* path, fuse_file_info* fi) {

    return 0;
}


void* Alipan::fsInit(fuse_conn_info* conn, fuse_config* cfg) {
    
    return nullptr;
}


void Alipan::fsDestroy(void* privateData) {


    
}


int Alipan::fsAccess(const char* path, int mask) {


    return 0;
}


int Alipan::fsCreate(const char* path, mode_t mode, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


bool Alipan::isLoggedIn() {
    return !this->oauthCode.empty();
}


bool Alipan::isNotLoggedIn() {
    return !this->isLoggedIn();
}





}  // namespace cloudland
}  // namespace fs

