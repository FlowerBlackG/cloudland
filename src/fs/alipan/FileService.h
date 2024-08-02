// SPDX-License-Identifier: MulanPSL-2.0
/*
    File service for Alipan driver.

    created on 2024.8.2 at Haidian, Beijing

*/


#pragma once

#include <nlohmann/json.hpp>
#include <memory>
#include <thread>
#include <mutex>

#include "./MiniDB.h"


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

        std::mutex db;
    } locks;
};



}  // namespace alipan
}  // namespace cloudland
}  // namespace fs

