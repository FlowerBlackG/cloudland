// SPDX-License-Identifier: MulanPSL-2.0
/*
    mirror fs

    created on 2024.7.26 at Haidian, Beijing

*/



#include "./Mirror.h"
#include "../../GlobalData.h"

#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


using namespace std;

namespace cloudland {
namespace fs {


CLOUDLAND_FS_PREPARE_CLASS_CPP(Mirror)





int Mirror::fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) {
    LOG_INFO("mirror getattr")

    auto res = lstat((globalData.dataDir + path).c_str(), st);
    if (res == -1)
        return -errno;

    return 0;
}


int Mirror::fsReadLink(const char* path, char* buf, size_t size) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsMkdir(const char* path, mode_t mode) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsUnlink(const char* path) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsRmdir(const char* path) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsSymlink(const char* from, const char* to) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsRename(const char* from, const char* to, unsigned int flags) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsLink(const char* from, const char* to) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsTruncate(const char* path, off_t size, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsOpen(const char* path, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsStatfs(const char* path, struct statvfs* stbuf) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsSetXAttr(const char* path, const char* name, const char* value, size_t size, int flags) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsGetXAttr(const char* path, const char* name, char* value, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsListXAttr(const char* path, char* list, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Mirror::fsOpenDir(const char* path, fuse_file_info* fi) {


    LOG_INFO("mirror opendir")
    return 0;
}


int Mirror::fsReadDir(
    const char* path, 
    void* buf, 
    fuse_fill_dir_t filler, 
    off_t offset, 
    fuse_file_info* fi, 
    fuse_readdir_flags flags
) {

    LOG_INFO("mirror readdir")

    DIR* dp;
    dp = opendir((globalData.dataDir + path).c_str());
    if (dp == nullptr) {
        return -errno;
    }

    while (dirent* de = readdir(dp)) {
        struct stat st {0};
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0, FUSE_FILL_DIR_PLUS)) {
            break;
        }
    }

    closedir(dp);

    return 0;
}



int Mirror::fsReleaseDir(const char* path, fuse_file_info* fi) {

    LOG_INFO("mirror releasedir")
    return 0;
}


void* Mirror::fsInit(fuse_conn_info* conn, fuse_config* cfg) {
    
    return nullptr;
}


void Mirror::fsDestroy(void* privateData) {


    
}


int Mirror::fsAccess(const char* path, int mask) {

    LOG_INFO("mirror access")

    auto res = access((globalData.dataDir + path).c_str(), mask);
    if (res == -1) {
        return -errno;
    }

    return 0;
}


int Mirror::fsCreate(const char* path, mode_t mode, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}





}  // namespace cloudland
}  // namespace fs

