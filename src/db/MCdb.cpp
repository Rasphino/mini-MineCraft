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
        case MYSQL: {
            connection = mysql_init(NULL); // 初始化数据库连接变量
            if (connection == nullptr) {
                cout << "Error:" << mysql_error(connection);
                exit(1);
            }
            execSQL("create database if not exists MineCraft");
            break;
        }

        case SQLITE: {
            // Skip, nothing to do.
            break;
        }
    }
}

MCdb::~MCdb() {
    switch (type) {
        case MYSQL: {
            if (connection != nullptr)  // 关闭数据库连接
                mysql_close(connection);
            break;
        }

        case SQLITE: {
            if (db != nullptr)
                sqlite3_close(db);
            break;
        }
    }
}

bool MCdb::initDB(std::string db_name, std::string host, std::string user, std::string passwd) {
    switch (type) {
        case MYSQL: {
            // 函数mysql_real_connect建立一个数据库连接
            // 成功返回MYSQL*连接句柄，失败返回NULL
            connection = mysql_real_connect(connection, host.c_str(),
                                            user.c_str(), passwd.c_str(), db_name.c_str(), 0, NULL,
                                            0);
            if (connection == nullptr) {
                cerr << "Error:" << mysql_error(connection);
                exit(1);
            }
            break;
        }

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
        case MYSQL: {
            // mysql_query()执行成功返回0，失败返回非0值。与PHP中不一样
            if (mysql_real_query(connection, sql.c_str(), sql.size())) {
                cerr << "Query Error:" << mysql_error(connection);
                // return false;
                exit(1);
            }
            break;
        }

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
        case MYSQL: {
            // mysql_query()执行成功返回0，失败返回非0值。与PHP中不一样
            if (mysql_real_query(connection, sql.c_str(), sql.size())) {
                cerr << "Query Error:" << sql << "[" << mysql_error(connection) << "]\n";
                // return false;
                exit(1);
            }
            MYSQL_RES *res = mysql_store_result(connection);
            for (int i = 0; i < mysql_num_rows(res); i++) {
                MYSQL_ROW row = mysql_fetch_row(res);
                Record t;
                for (int j = 0; j < res->field_count; j++) {
                    t.push_back(row[j]);
                }
                r.push_back(t);
            }
            break;
        }

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
        case MYSQL: {
            // mysql_query()执行成功返回0，失败返回非0值。与PHP中不一样
            if (mysql_real_query(connection, sql, strlen(sql))) {
                cerr << "Query Error:" << mysql_error(connection);
                // return false;
                exit(1);
            }
            break;
        }

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
        case MYSQL: {
            // mysql_query()执行成功返回0，失败返回非0值。与PHP中不一样
            if (mysql_real_query(connection, sql, strlen(sql))) {
                cerr << "Query Error:" << sql << "[" << mysql_error(connection) << "]\n";
                // return false;
                exit(1);
            }
            MYSQL_RES *res = mysql_store_result(connection);
            for (int i = 0; i < mysql_num_rows(res); i++) {
                MYSQL_ROW row = mysql_fetch_row(res);
                Record t;
                for (int j = 0; j < res->field_count; j++) {
                    t.push_back(row[j]);
                }
                r.push_back(t);
            }
            break;
        }

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

