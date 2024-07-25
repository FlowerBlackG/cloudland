// SPDX-License-Identifier: MulanPSL-2.0
/*
    cloudland entry.

    created on 2024.7.25 at Haidian, Beijing

*/

#include <iostream>
#include <cstdio>
#include <map>
#include <set>
#include <vector>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <fuse3/fuse.h>

#include "./utils/Log.h"
#include "./config.h"


using namespace std;


/* ------------ global variables ------------ */

/**
 * arguments passed by externals
 */
static struct {
    /** example: --resolution-width 1920 */
    map<string, string> values;

    /** example: --headless */
    set<string> flags;
    
    /** 
     * arguments followed. 
     * for example: 
     *     lab.cpp and lab2.cpp
     *   in
     *     g++ -Wall -o lab lab.cpp lab2.cpp 
     */
    vector<string> paimons;
    
    /** environment variables. */
    map<string, string> env;
} args;


static struct {
    string fs;
    string mountPoint;
    string dataDir;
    bool daemonize;
} options;


/* ------------ args parser ------------ */


static struct {
    bool initialized = false;
    set<string> flagKeys;
    set<string> valueKeys;
} argKeys;

static void loadPredefinedArgKeys() {

    if (argKeys.initialized) {
        return;
    }

    struct {
        const char* key;
        bool isFlag = false;
    } keys[] = {
        { "--version", true },
        { "--usage", true },
        { "--help", true },


        { "--no-color", true },
        { "--log-to", false },

        { "--fs", false },
        { "--mount-point", false },
        { "--data-dir", false },
        { "--daemonize", true }
        
    };

    for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        auto& it = keys[i];
        (it.isFlag ? &argKeys.flagKeys : &argKeys.valueKeys)->insert(it.key);
    }

    argKeys.initialized = true;
}



static int parseArgs(int argc, const char** argv) {

    if (!argKeys.initialized) {
        loadPredefinedArgKeys();
    }

    const auto& isKey = [] (string& s) {
        return s.starts_with("--");
    };

    auto& flags = args.flags;
    auto& values = args.values;
    auto& paimons = args.paimons;


    for (int idx = 1; idx < argc; idx++) {
        string key = argv[idx];
        if (argKeys.flagKeys.contains(key)) {
            if (flags.contains(key)) {
                LOG_WARN("flag redefined: ", key);
            } else {
                flags.insert(key);
            }

            continue;
        } else if (argKeys.valueKeys.contains(key)) {
            if (idx + 1 == argc) {
                LOG_ERROR("no value for key ", key);
                return -1;
            }

            string value = argv[++idx];
            if (isKey(value)) {
                LOG_ERROR("key ", key, " followed by another key ", value);
                return -1;
            }

            if (values.contains(key)) {
                LOG_WARN("key ", key, " redefined. ");
                LOG_WARN("  value |", values[key], "| is replaced by |", value, "|.");
            }

            values[key] = value;
        } else if (isKey(key)) {
            LOG_ERROR("key ", key, " is not defined.")
            return -1;
        } else {
            paimons.push_back(key);
        }
    }


    return 0;
}

static void processEnvVars(const char** env) {
    for (int i = 0; env[i]; i++) {
        string it = env[i];
        auto splitPos = it.find('=');
        if (splitPos == string::npos) {
            LOG_WARN("no '=' in env variable: ", it);
            continue;
        }
        args.env[it.substr(0, splitPos)] = it.substr(splitPos + 1);
    }
}


/* ------------ methods ------------ */

static void usage() {
    cout << "cloudland " << CLOUDLAND_VERSION_NAME << endl;
    cout << "check ";
    ConsoleColorPad::setCoutColor(0x2f, 0x90, 0xb9);
    cout << "https://github.com/FlowerBlackG/cloudland";
    ConsoleColorPad::setCoutColor();
    cout << " for detailed usage." << endl;
}



static void version(bool useLog) {
    if (useLog) {
        LOG_INFO(
            "cloudland version : ", 
            CLOUDLAND_VERSION_NAME, 
            " (", CLOUDLAND_VERSION_CODE,")"
        );

        LOG_INFO(
            "fuse version: ", FUSE_MAJOR_VERSION, ".", FUSE_MINOR_VERSION
        );

        LOG_INFO("cloudland complication time: ", CLOUDLAND_BUILD_TIME_HUMAN_READABLE);


    } else {
        
        cout << "cloudland " << CLOUDLAND_VERSION_NAME 
            << " (" << CLOUDLAND_VERSION_CODE << ")" << endl
            << "" << CLOUDLAND_BUILD_TIME_HUMAN_READABLE << endl;
        cout << "------------" << endl;
        cout << "fuse: " << FUSE_MAJOR_VERSION << "." << FUSE_MINOR_VERSION << endl;
    
    }
}


static int processPureQueryCmds() {
    if (args.flags.contains("--version")) {
        version(false);
        return 1;
    } 
    
    if (args.flags.contains("--usage") || args.flags.contains("--help")) {
        usage();
        return 1;
    }
    return 0;
}


static int buildOptions() {

    if (args.values.contains("--mount-point")) {
        options.mountPoint = args.values["--mount-point"];
    } else {
        LOG_ERROR("cmdline --mount-point is required.")
        usage();
        return -1;
    }


    if (args.values.contains("--data-dir")) {
        options.dataDir = args.values["--data-dir"];
    } else {
        LOG_ERROR("cmdline --data-dir is required.")
        usage();
        return -1;
    }


    if (args.values.contains("--fs")) {
        options.fs = args.values["--fs"];
    } else {
        LOG_ERROR("cmdline --fs is required.")
        usage();
        return -1;
    }


    options.daemonize = args.flags.contains("--daemonize");

    return 0;
};


static int buildFuseArgs(vector<string>& argvS, vector<char*>& argv) {

    if (!options.daemonize) {
        argvS.push_back("-f");
    }


    argvS.push_back("--");
    argvS.push_back(options.mountPoint);


    // build char*[]

    for (auto& it : argvS) {
        argv.push_back(it.data());
    }

    return 0;
}


static void cleanup() {

}


int main(int argc, const char* argv[], const char* env[]) {

    ConsoleColorPad::disableColor();

    /* parse cmdlines */

    if (int res = parseArgs(argc, argv)) {
        usage();
        return res;
    }

    ConsoleColorPad::setNoColor(args.flags.contains("--no-color"));
    if (args.values.contains("--log-to")) {
        string& logTo = args.values["--log-to"];
        freopen(logTo.c_str(), "w", stderr);
    }

    processEnvVars(env);

    if (processPureQueryCmds()) {
        return 0;
    }

    if (int res = buildOptions()) { 
        usage(); 
        return res; 
    }


    fuse_operations fs {0};
    fs.readdir = [] (
        const char * path, 
        void * buffer, 
        fuse_fill_dir_t filler, 
        off_t offset,
        fuse_file_info * fi,
        fuse_readdir_flags flags
    ) {

        struct stat st;
        st.st_ino = 999999;
        st.st_mode = 0777;
        filler(buffer, "example", &st, 0, FUSE_FILL_DIR_PLUS);

        return 0;
    };

    fs.getattr = [] (
        const char* path,
        struct stat* stbuf,
        fuse_file_info* fi
    ) {
        return 0;
    };


    vector<string> fuseArgVS;
    vector<char*> fuseArgV;

    if (int res = buildFuseArgs(fuseArgVS, fuseArgV)) {
        LOG_ERROR("failed to build fuse arguments!");
        return res;
    }

    auto fuseResCode = fuse_main(fuseArgV.size(), fuseArgV.data(), &fs, (void*)0xfb);

    cleanup();
    return fuseResCode;
}

