// SPDX-License-Identifier: MulanPSL-2.0
/*
    File Tree and related structures

    created on 2024.8.7 at Haidian, Beijing

*/


#include "./FileTree.h"

#include <iostream>

using namespace std;


namespace cloudland::fs::alipan {


bool FileTree::isFile() {
    return this->attr.isFile;
}


bool FileTree::isFolder() {
    return this->attr.isFolder;
}



FileTree& FileTree::addChild(std::shared_ptr<FileTree> pTree) {
    this->children.push_back(pTree);

    return *this;
}


FileTree& FileTree::addChild(FileTree* tree) {
    this->children.emplace_back(tree);

    return *this;
}


}  // namespace cloudland::fs::alipan

