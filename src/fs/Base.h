// SPDX-License-Identifier: MulanPSL-2.0
/*
    cloudland fs driver.

    created on 2024.7.25 at Haidian, Beijing

*/

#pragma once

#include <fuse3/fuse.h>
#include "./CloudlandFSMacros.h"


namespace cloudland {
namespace fs {


class Base {
public:
    virtual int fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi);
    virtual int fsReadLink(const char* path, char* buf, size_t size);
    virtual int fsMkdir(const char* path, mode_t mode);
    virtual int fsUnlink(const char* path);
    virtual int fsRmdir(const char* path);
    virtual int fsSymlink(const char* from, const char* to);
    virtual int fsRename(const char* from, const char* to, unsigned int flags);
    virtual int fsLink(const char* from, const char* to);
    virtual int fsTruncate(const char* path, off_t size, fuse_file_info* fi);
    virtual int fsOpen(const char* path, fuse_file_info* fi);
    virtual int fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi);
    virtual int fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi);
    virtual int fsStatfs(const char* path, struct statvfs* stbuf);
    virtual int fsSetXAttr(const char* path, const char* name, const char* value, size_t size, int flags);
    virtual int fsGetXAttr(const char* path, const char* name, char* value, size_t size);
    virtual int fsListXAttr(const char* path, char* list, size_t size);
    virtual int fsOpenDir(const char* path, fuse_file_info* fi);
    virtual int fsReadDir(
        const char* path, 
        void* buf, 
        fuse_fill_dir_t filler, 
        off_t offset, 
        fuse_file_info* fi, 
        fuse_readdir_flags flags
    );
    virtual int fsReleaseDir(const char* path, fuse_file_info* fi);
    virtual void* fsInit(fuse_conn_info* conn, fuse_config* cfg);
    virtual void fsDestroy(void* privateData);
    virtual int fsAccess(const char* path, int mask);
    virtual int fsCreate(const char* path, mode_t mode, fuse_file_info* fi);




CLOUDLAND_FS_PREPARE_CLASS_HEADER()


};





} // namespace fs
} // namespace cloudland
