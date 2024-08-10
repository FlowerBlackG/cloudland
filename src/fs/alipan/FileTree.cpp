// SPDX-License-Identifier: MulanPSL-2.0
/*
    File Tree and related structures

    created on 2024.8.7 at Haidian, Beijing

*/


#include "./FileTree.h"

#include <iostream>

using namespace std;


namespace cloudland::fs::alipan {

static const string FILETREE_JSON_KEY_CHILDREN = "children";


FileTree::FileTree() {

}


FileTree::FileTree(const nlohmann::json& json) {
    nlohmann::json myJson = json;

    if (myJson.contains(FILETREE_JSON_KEY_CHILDREN)) {
        auto childrenArr = (vector<nlohmann::json>) myJson[FILETREE_JSON_KEY_CHILDREN];
        for (auto& it : childrenArr) {
            this->addChild(make_shared<FileTree>(it));
        }
        myJson.erase(FILETREE_JSON_KEY_CHILDREN);
    }

    this->attr.load(myJson);
}


FileTree::FileTree(const FileInfo& info) {
    this->attr = info;
}


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


nlohmann::json FileTree::toJson() {
    nlohmann::json res = attr.getJson();
    res[FILETREE_JSON_KEY_CHILDREN] = vector<nlohmann::json> {};
    for (auto& it : children) {
        auto json = it->toJson();
        res[FILETREE_JSON_KEY_CHILDREN].push_back(json);
    }

    return res;
}


}  // namespace cloudland::fs::alipan

