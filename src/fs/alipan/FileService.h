// SPDX-License-Identifier: MulanPSL-2.0
/*
    File service for Alipan driver.

    created on 2024.8.2 at Haidian, Beijing

*/


/*
    File Service for Aliyun Drive (Alipan).

    File Service keeps files stored locally organized, 
    provides cache for file attributes and keep local clone
    of the cloud drive updated.

    File tree and files' attributes are stored in json format,
    which would be saved to minidb.

    File Service creates data folder for each cloud drive (recognized by drive id)
    in the global data directory. For example, the data-dir might looks like this:

    -- data-dir
      -> minidb.json  # minidb's dump
      -> 68868711  # data dir for alipan drive whose id is 68868711
      -> 22796584  # data dir for alipan drive whose id is 22796584
      ...
 */

#pragma once

#include <nlohmann/json.hpp>
#include <memory>
#include <thread>
#include <mutex>
#include <map>

#include "./MiniDB.h"
#include "./FileTree.h"


namespace cloudland {
namespace fs {
namespace alipan {



class FileService {
public:

    enum class LoopStatus {
        READY, RUNNING, STOPPED
    };


    FileService(std::shared_ptr<MiniDB> miniDB);

    ~FileService();
    void dumpToMiniDB();

    int startLoop();
    void stopLoop(bool join);


protected:
    void loop();


protected:
    nlohmann::json db;


    std::shared_ptr<MiniDB> miniDB;
    std::shared_ptr<std::thread> loopThread; 

    LoopStatus loopStatus = LoopStatus::READY;

    struct {
        std::recursive_mutex db;
    } locks;
};



}  // namespace alipan
}  // namespace fs
}  // namespace cloudland

