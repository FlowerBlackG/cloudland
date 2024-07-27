// SPDX-License-Identifier: MulanPSL-2.0
/*
    cloudland fs driver.

    created on 2024.7.25 at Haidian, Beijing

*/


#include "./FsDriverBase.h"

#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>


namespace cloudland {
namespace fs {


FsDriverBase::FsDriverBase() {
    
}


FsDriverBase::~FsDriverBase() {
    
}


int FsDriverBase::fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsReadLink(const char* path, char* buf, size_t size) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsMkdir(const char* path, mode_t mode) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsUnlink(const char* path) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsRmdir(const char* path) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsSymlink(const char* from, const char* to) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsRename(const char* from, const char* to, unsigned int flags) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsLink(const char* from, const char* to) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsTruncate(const char* path, off_t size, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsOpen(const char* path, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsStatfs(const char* path, struct statvfs* stbuf) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsSetXAttr(const char* path, const char* name, const char* value, size_t size, int flags) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsGetXAttr(const char* path, const char* name, char* value, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsListXAttr(const char* path, char* list, size_t size) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsOpenDir(const char* path, fuse_file_info* fi) {


    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsReadDir(
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


int FsDriverBase::fsReleaseDir(const char* path, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


void* FsDriverBase::fsInit(fuse_conn_info* conn, fuse_config* cfg) {
    
    return nullptr;
}


void FsDriverBase::fsDestroy(void* privateData) {


    
}


int FsDriverBase::fsAccess(const char* path, int mask) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


int FsDriverBase::fsCreate(const char* path, mode_t mode, fuse_file_info* fi) {

    LOG_ERROR("Method not implemented.")
    return -ENOSYS;
}


fuse_operations* FsDriverBase::getFuseOperations() {
    return nullptr;
}



} // namespace fs
} // namespace cloudland
