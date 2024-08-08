// SPDX-License-Identifier: MulanPSL-2.0
/*
    File Tree and related structures

    created on 2024.8.4 at Haidian, Beijing

*/

#pragma once


#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include "./FileInfo.h"


namespace cloudland::fs::alipan {


struct FileTree {
    FileInfo attr;
    
    bool isFile();
    bool isFolder();

    FileTree& addChild(std::shared_ptr<FileTree>);

    /**
     * 
     * Memory of the FileTree object will be managed, so don't delete it yourself. 
     */
    FileTree& addChild(FileTree*);

    std::vector<std::shared_ptr<FileTree>> children;
};


}  // namespace cloudland::fs::alipan

