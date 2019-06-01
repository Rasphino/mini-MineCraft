//
//  MCdb.cpp
//  db
//
//  Created by jones on 2019/5/22.
//  Copyright © 2019 jones. All rights reserved.
//

#include "MCdb.h"
#include<iostream>
#include<cstdlib>
#include <cstring>

using namespace std;


MCdb::MCdb(MCdb::Type t) : type(t) {
    switch (t) {
        case SQLITE: {
            // Skip, nothing to do.
            break;
        }
    }
}

MCdb::~MCdb() {
    switch (type) {
        case SQLITE: {
            if (db != nullptr)
                sqlite3_close(db);
            break;
        }
    }
}

bool MCdb::initDB(std::string db_name, std::string host, std::string user, std::string passwd) {
    switch (type) {
        case SQLITE: {
            rc = sqlite3_open(db_name.c_str(), &db);
            if (rc) {
                cerr << "Error:" << sqlite3_errmsg(db);
                exit(1);
            }
            break;
        }
    }

    return true;
}

bool MCdb::execSQL(string &sql) {
    switch (type) {
        case SQLITE: {
            rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
            if (rc != SQLITE_OK) {
                cerr << "SQL error: " << sqlite3_errmsg(db) << "\n";
                // return false;
                exit(1);
            }
            break;
        }
    }

    return true;
}

int MCdb::sqliteCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
    auto *records = static_cast<Records *>(p_data);
    try {
        records->emplace_back(p_fields, p_fields + num_fields);
    }
    catch (...) {
        // abort select on failure, don't let exception propogate thru sqlite3 call-stack
        return 1;
    }
    return 0;
}

bool MCdb::execSQL(std::string &sql, Records &r) {
    switch (type) {
        case SQLITE: {
            char *errMsg;
            rc = sqlite3_exec(db, sql.c_str(), sqliteCallback, &r, &errMsg);
            if (rc != SQLITE_OK) {
                cerr << "SQL error: " << sql << "[" << errMsg << "]\n";
                // return false;
                exit(1);
            }
            break;
        }
    }
    return true;
}

bool MCdb::execSQL(char *sql) {
    switch (type) {
        case SQLITE: {
            rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
            if (rc != SQLITE_OK) {
                cerr << "SQL error: " << sqlite3_errmsg(db) << "\n";
                // return false;
                exit(1);
            }
            break;
        }
    }

    return true;
}

bool MCdb::execSQL(char *sql, Records &r) {
    switch (type) {
        case SQLITE: {
            char *errMsg;
            rc = sqlite3_exec(db, sql, sqliteCallback, &r, &errMsg);
            if (rc != SQLITE_OK) {
                cerr << "SQL error: " << sql << "[" << errMsg << "]\n";
                // return false;
                exit(1);
            }
            break;
        }
    }
    return true;
}

