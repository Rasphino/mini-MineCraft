//
//  MCdb.h
//  db
//
//  Created by jones on 2019/5/22.
//  Copyright © 2019 jones. All rights reserved.
//

#ifndef MCdb_h
#define MCdb_h

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using Record = std::vector<std::string>;
using Records = std::vector<Record>;

class MCdb {
public:
    enum Type {
        SQLITE
    };
    Type type;
    explicit MCdb(Type t = Type::SQLITE);
    ~MCdb();
    bool initDB(std::string db_name, std::string host = "localhost", std::string user = "",
                std::string passwd = "");
    bool execSQL(std::string &sql);
    bool execSQL(std::string &sql, Records &r);
    bool execSQL(char *sql);
    bool execSQL(char *sql, Records &r);
private:
    // Sqlite part
    ::sqlite3 *db;
    int rc;
    static int sqliteCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);
};


//inline void insert_query(MCdb db,int chunkid, int i, int t, int k, int type) {
//    string insert_query =
//    "insert into MineCraft.block" + to_string(chunkid) + " ("
//    "select "
//    + to_string(chunkid) + ","
//    + to_string(i) +"," + to_string(t) + ","+ to_string(k)
//    + "," + to_string(int(0)) + " "
//    "from dual where not exists (select * from MineCraft.block" + to_string(chunkid) +
//    + " where chunkid = " + to_string(chunkid) + " and "
//    + "x = " + to_string(i) + " and "
//    + "y = " + to_string(t) + " and "
//    + "z = " + to_string(k) + " and "
//    + "chunkType = " + to_string(int(0))
//    + "));";
//    db.execSQL(insert_query);
//}


#endif /* MCdb_h */
