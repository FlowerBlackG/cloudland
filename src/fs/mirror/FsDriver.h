// SPDX-License-Identifier: MulanPSL-2.0
/*
    mirror fs

    this fs is used for test purpose.

    created on 2024.7.26 at Haidian, Beijing

*/


#pragma once

#include "../FsDriverBase.h"


namespace cloudland {
namespace fs {
namespace mirror {


class FsDriver : public FsDriverBase {

public:
    virtual int fsGetAttr(const char* path, struct stat* st, fuse_file_info* fi) override;
    virtual int fsReadLink(const char* path, char* buf, size_t size) override;
    virtual int fsMkdir(const char* path, mode_t mode) override;
    virtual int fsUnlink(const char* path) override;
    virtual int fsRmdir(const char* path) override;
    virtual int fsSymlink(const char* from, const char* to) override;
    virtual int fsRename(const char* from, const char* to, unsigned int flags) override;
    virtual int fsLink(const char* from, const char* to) override;
    virtual int fsTruncate(const char* path, off_t size, fuse_file_info* fi) override;
    virtual int fsOpen(const char* path, fuse_file_info* fi) override;
    virtual int fsRead(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi) override;
    virtual int fsWrite(const char* path, const char* buf, size_t size, off_t offset, fuse_file_info* fi) override;
    virtual int fsStatfs(const char* path, struct statvfs* stbuf) override;
    virtual int fsSetXAttr(const char* path, const char* name, const char* value, size_t size, int flags) override;
    virtual int fsGetXAttr(const char* path, const char* name, char* value, size_t size) override;
    virtual int fsListXAttr(const char* path, char* list, size_t size) override;
    virtual int fsOpenDir(const char* path, fuse_file_info* fi) override;
    virtual int fsReadDir(
        const char* path, 
        void* buf, 
        fuse_fill_dir_t filler, 
        off_t offset, 
        fuse_file_info* fi, 
        fuse_readdir_flags flags
    ) override;
    virtual int fsReleaseDir(const char* path, fuse_file_info* fi) override;
    virtual void* fsInit(fuse_conn_info* conn, fuse_config* cfg) override;
    virtual void fsDestroy(void* privateData) override;
    virtual int fsAccess(const char* path, int mask) override;
    virtual int fsCreate(const char* path, mode_t mode, fuse_file_info* fi) override;
    


CLOUDLAND_FS_PREPARE_CLASS_HEADER()


};



}  // namespace mirror
}  // namespace cloudland
}  // namespace fs

