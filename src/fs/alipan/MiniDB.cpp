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


void MiniDB::load(istream& in) {
    in >> this->db;
}


void MiniDB::dump(ostream& out) {
    out << this->db;
}


bool MiniDB::contains(const string& key) {
    return this->db.contains(key);
}



void MiniDB::erase(const std::string& key) {
    this->db.erase(key);
}


int64_t MiniDB::getLong(const string& key) {
    return (int64_t) this->db[key];
}


int64_t MiniDB::getLong(const string& key, int64_t fallback) {
    return contains(key) ? getLong(key) : fallback;
}



string MiniDB::getString(const string& key) {
    return (string) this->db[key];
}


string MiniDB::getString(const string& key, const string& fallback) {
    return contains(key) ? getString(key) : fallback;
}



}  // namespace alipan
}  // namespace cloudland
}  // namespace fs

