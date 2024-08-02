// SPDX-License-Identifier: MulanPSL-2.0
/*
    File service for Alipan driver.

    created on 2024.8.2 at Haidian, Beijing

*/


#include "./FileService.h"
#include "../../utils/Log.h"

using namespace std;


namespace cloudland {
namespace fs {
namespace alipan {


FileService::FileService(shared_ptr<MiniDB> miniDB) {
    this->miniDB = miniDB;
}


FileService::~FileService() {
    this->dumpToMiniDB();

    if (this->loopThread) {
        this->loopThread->join();
    }
}


void FileService::dumpToMiniDB() {
    if (miniDB == nullptr) {
        return;
    }

    {
        lock_guard<mutex> lock {this->locks.db};
        miniDB->db[MiniDBKey::FILE_SERVICE_DATA] = this->db;
    }
}


int FileService::startLoop() {
    if (loopStatus != LoopStatus::READY) {
        return 1;
    }

    loopStatus = LoopStatus::RUNNING;
    this->loopThread = make_shared<thread>(&FileService::loop, this);

    return this->loopThread ? 0 : 2;
}


void FileService::stopLoop(bool join) {
    if (loopStatus != LoopStatus::RUNNING) 
        return;

    loopStatus = LoopStatus::STOPPED;
    if (join)
        this->loopThread->join();
}


void FileService::loop() {
    while (loopStatus == LoopStatus::RUNNING) {
        LOG_INFO("there!"); // todo
        this_thread::sleep_for(1000ms);
    }
}


}  // namespace alipan
}  // namespace cloudland
}  // namespace fs

