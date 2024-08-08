// SPDX-License-Identifier: MulanPSL-2.0
/*
    minidb for alipan fs driver

    created on 2024.8.2 at Haidian, Beijing

*/


#include <memory>
#include "./MiniDB.h"



using namespace std;


namespace cloudland {
namespace fs {
namespace alipan {


static unique_ptr<MiniDB> instanceHolder;
    

// minidb keys' definitions

string MiniDBKey::OAUTH_ACCESS_TOKEN = "oauth-access-token";
string MiniDBKey::OAUTH_REFRESH_TOKEN = "oauth-refresh-token";
string MiniDBKey::OAUTH_ACCESS_TOKEN_EXPIRE_TIME_SEC = "oauth-access-token-expire-time-sec";

string MiniDBKey::USERID = "alipan-userid";
string MiniDBKey::DEFAULT_DRIVE_ID = "alipan-default-drive-id";
string MiniDBKey::RESOURCE_DRIVE_ID = "alipan-resource-drive-id";
string MiniDBKey::BACKUP_DRIVE_ID = "alipan-backup-drive-id";

string MiniDBKey::FILE_SERVICE_DATA = "file-service-data";

#define CLOUDLAND_MINIDB_USE_LOCK() \
    lock_guard<recursive_mutex> __cloudland_minidb_use_lock_lock_guard {this->dbLock}


MiniDB::~MiniDB() {
    CLOUDLAND_MINIDB_USE_LOCK();
}


void MiniDB::load(istream& in) {
    CLOUDLAND_MINIDB_USE_LOCK();
    in >> this->db;
}


void MiniDB::dump(ostream& out) {
    CLOUDLAND_MINIDB_USE_LOCK();
    out << this->db;
}


bool MiniDB::contains(const string& key) {
    CLOUDLAND_MINIDB_USE_LOCK();
    return this->db.contains(key);
}



void MiniDB::erase(const std::string& key) {
    CLOUDLAND_MINIDB_USE_LOCK();
    if (this->db.contains(key))
        this->db.erase(key);
}


int64_t MiniDB::getLong(const string& key) {
    CLOUDLAND_MINIDB_USE_LOCK();
    return (int64_t) this->db[key];
}


int64_t MiniDB::getLong(const string& key, int64_t fallback) {
    CLOUDLAND_MINIDB_USE_LOCK();
    return db.contains(key) ? getLong(key) : fallback;
}



string MiniDB::getString(const string& key) {
    CLOUDLAND_MINIDB_USE_LOCK();
    return (string) this->db[key];
}


string MiniDB::getString(const string& key, const string& fallback) {
    CLOUDLAND_MINIDB_USE_LOCK();
    return db.contains(key) ? getString(key) : fallback;
}




void MiniDB::withLock(function<void()>& func) {
    CLOUDLAND_MINIDB_USE_LOCK();
    func();
}


void MiniDB::useDB(const std::function<void(nlohmann::json&)>& func) {
    CLOUDLAND_MINIDB_USE_LOCK();
    func(this->db);
}


}  // namespace alipan
}  // namespace fs
}  // namespace cloudland

