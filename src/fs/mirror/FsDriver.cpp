// SPDX-License-Identifier: MulanPSL-2.0
/*
    mirror fs

    created on 2024.7.26 at Haidian, Beijing

*/



#include "./FsDriver.h"
#include "../../GlobalData.h"

#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


using namespace std;

namespace cloudland {
namespace fs {
namespace mirror {



CLOUDLAND_FS_PREPARE_CLASS_CPP()



int FsDriver::fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) {
    LOG_INFO("mirror getattr")

    auto res = lstat((globalData.dataDir + path).c_str(), st);
    if (res == -1)
        return -errno;

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


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriver::fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi) {

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


    LOG_INFO("mirror opendir")
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



int FsDriver::fsReleaseDir(const char* path, fuse_file_info* fi) {

    LOG_INFO("mirror releasedir")
    return 0;
}


void* FsDriver::fsInit(fuse_conn_info* conn, fuse_config* cfg) {
    
    return nullptr;
}


void FsDriver::fsDestroy(void* privateData) {


    
}


int FsDriver::fsAccess(const char* path, int mask) {

    LOG_INFO("mirror access")

    auto res = access((globalData.dataDir + path).c_str(), mask);
    if (res == -1) {
        return -errno;
    }

    return 0;
}


int FsDriver::fsCreate(const char* path, mode_t mode, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}





}  // namespace mirror
}  // namespace cloudland
}  // namespace fs

