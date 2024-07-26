// SPDX-License-Identifier: MulanPSL-2.0
/*
    cloudland fs driver.

    created on 2024.7.25 at Haidian, Beijing

*/


#include "./Base.h"

#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>


namespace cloudland {
namespace fs {

int Base::fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsReadLink(const char* path, char* buf, size_t size) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsMkdir(const char* path, mode_t mode) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsUnlink(const char* path) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsRmdir(const char* path) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsSymlink(const char* from, const char* to) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsRename(const char* from, const char* to, unsigned int flags) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsLink(const char* from, const char* to) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsTruncate(const char* path, off_t size, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsOpen(const char* path, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsStatfs(const char* path, struct statvfs* stbuf) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsSetXAttr(const char* path, const char* name, const char* value, size_t size, int flags) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsGetXAttr(const char* path, const char* name, char* value, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsListXAttr(const char* path, char* list, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsOpenDir(const char* path, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsReadDir(
    const char* path, 
    void* buf, 
    fuse_fill_dir_t filler, 
    off_t offset, 
    fuse_file_info* fi, 
    fuse_readdir_flags flags
) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsReleaseDir(const char* path, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


void* Base::fsInit(fuse_conn_info* conn, fuse_config* cfg) {
    
    return nullptr;
}


void Base::fsDestroy(void* privateData) {


    
}


int Base::fsAccess(const char* path, int mask) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int Base::fsCreate(const char* path, mode_t mode, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}



CLOUDLAND_FS_PREPARE_CLASS_CPP(Base) // todo



} // namespace fs
} // namespace cloudland
