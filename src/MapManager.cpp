//
// Created by rasp on 19-5-29.
//

#include <utility>
#include <tuple>
#include "MapManager.h"


MapManager::MapManager() {
    cache = new Cache[CHUNK_NUM];
    db = new MCdb(MCdb::Type::SQLITE);
    db->initDB("MineCraft");
}

MapManager::MapManager(glm::vec3 &pos) {
    cache = new Cache[CHUNK_NUM];
    db = new MCdb(MCdb::Type::SQLITE);
    db->initDB("MineCraft");
    genCacheMap(pos);
    p = pos;
    genCacheFromNoise();
}

MapManager::~MapManager() {
    delete[] cache;
    delete db;
}

void MapManager::genCacheMap(glm::vec3 &pos) {
    cacheMap[0][0] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2] - CHUNK_SIZE));
    cacheMap[0][1] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2]));
    cacheMap[0][2] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2] + CHUNK_SIZE));
    cacheMap[1][0] = getID(int(pos[0]), int(pos[2] - CHUNK_SIZE));
    cacheMap[1][1] = getID(int(pos[0]), int(pos[2]));
    cacheMap[1][2] = getID(int(pos[0]), int(pos[2] + CHUNK_SIZE));
    cacheMap[2][0] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2] - CHUNK_SIZE));
    cacheMap[2][1] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2]));
    cacheMap[2][2] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2] + CHUNK_SIZE));
//    cacheMap[0][0] = getID(int(pos[0] - 2 * CHUNK_SIZE), int(pos[2] - 2 * CHUNK_SIZE));
//    cacheMap[0][1] = getID(int(pos[0] - 2 * CHUNK_SIZE), int(pos[2] - CHUNK_SIZE));
//    cacheMap[0][2] = getID(int(pos[0] - 2 * CHUNK_SIZE), int(pos[2]));
//    cacheMap[0][3] = getID(int(pos[0] - 2 * CHUNK_SIZE), int(pos[2] + CHUNK_SIZE));
//    cacheMap[0][4] = getID(int(pos[0] - 2 * CHUNK_SIZE), int(pos[2] + 2 * CHUNK_SIZE));
//
//    cacheMap[1][0] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2] - 2 * CHUNK_SIZE));
//    cacheMap[1][1] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2] - CHUNK_SIZE));
//    cacheMap[1][2] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2]));
//    cacheMap[1][3] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2] + CHUNK_SIZE));
//    cacheMap[1][4] = getID(int(pos[0] - CHUNK_SIZE), int(pos[2] + 2 * CHUNK_SIZE));
//
//    cacheMap[2][0] = getID(int(pos[0]), int(pos[2] - 2 * CHUNK_SIZE));
//    cacheMap[2][1] = getID(int(pos[0]), int(pos[2] - CHUNK_SIZE));
//    cacheMap[2][2] = getID(int(pos[0]), int(pos[2]));
//    cacheMap[2][3] = getID(int(pos[0]), int(pos[2] + CHUNK_SIZE));
//    cacheMap[2][4] = getID(int(pos[0]), int(pos[2] + 2 * CHUNK_SIZE));
//
//    cacheMap[3][0] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2] - 2 * CHUNK_SIZE));
//    cacheMap[3][1] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2] - CHUNK_SIZE));
//    cacheMap[3][2] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2]));
//    cacheMap[3][3] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2] + CHUNK_SIZE));
//    cacheMap[3][4] = getID(int(pos[0] + CHUNK_SIZE), int(pos[2] + 2 * CHUNK_SIZE));
//
//    cacheMap[4][0] = getID(int(pos[0] + 2 * CHUNK_SIZE), int(pos[2] - 2 * CHUNK_SIZE));
//    cacheMap[4][1] = getID(int(pos[0] + 2 * CHUNK_SIZE), int(pos[2] - CHUNK_SIZE));
//    cacheMap[4][2] = getID(int(pos[0] + 2 * CHUNK_SIZE), int(pos[2]));
//    cacheMap[4][3] = getID(int(pos[0] + 2 * CHUNK_SIZE), int(pos[2] + CHUNK_SIZE));
//    cacheMap[4][4] = getID(int(pos[0] + 2 * CHUNK_SIZE), int(pos[2] + 2 * CHUNK_SIZE));
}

void MapManager::updateCacheMap(glm::vec3 &pos) {
    // if pos and lastPos are in the same chunk, no need to update cache map
    if (getID(int(p[0]), int(p[2])) == getID(int(pos[0]), int(pos[2]))) {
        return;
    }
    std::clog << p[0] << p[2] << std::endl;

    genCacheMap(pos);
    p = pos;
    genCacheFromNoise();
}

void MapManager::genCacheFromNoise() {
    int x, z;
    std::tie(x, z) = getChunkVertex(p[0], p[2]);

    x = (x - CHUNK_NUM / 2) * CHUNK_SIZE;
    z = (z - CHUNK_NUM / 2) * CHUNK_SIZE;

    for (int cx = 0; cx < CHUNK_NUM; ++cx) {
        for (int cz = 0; cz < CHUNK_NUM; ++cz) {
            for (int i = 0; i < CHUNK_SIZE; ++i) {
                for (int k = 0; k < CHUNK_SIZE; ++k) {
                    int h = (int) ((n.PerlinNoise((cx * CHUNK_SIZE + x + i) * 0.1,
                                                  (cz * CHUNK_SIZE + z + k) * 0.1) + 1) * 10);
                    for (int j = 0; j < h; ++j) {
                        (*cache)[cx][cz][i][j][k] = CubeType::SOIL;
                    }
                    for (int j = h; j < 64; ++j) {
                        (*cache)[cx][cz][i][j][k] = CubeType::NONE;
                    }
                    (*cache)[cx][cz][i][h][k] = CubeType::GRASS;
                }
            }

            loadFlower(cx, cz);
        }
    }
}

Cache *MapManager::getCache() {
    return cache;
}

std::pair<int32_t, int32_t> MapManager::getCacheVertexCoord() {
    int x, z;
    std::tie(x, z) = getChunkVertex(p[0], p[2]);
    x = x - CHUNK_NUM / 2;
    z = z - CHUNK_NUM / 2;
    return std::pair<int32_t, int32_t>(x * CHUNK_SIZE, z * CHUNK_SIZE);
}

void MapManager::genFlower(int cx, int cz) {
    int x, z;
    std::tie(x, z) = getChunkVertex(p[0], p[2]);
    x = (x - CHUNK_NUM / 2) * CHUNK_SIZE;
    z = (z - CHUNK_NUM / 2) * CHUNK_SIZE;

    std::vector<std::string> querys;

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int k = 0; k < CHUNK_SIZE; ++k) {
            int h = (int) ((n.PerlinNoise((cx * CHUNK_SIZE + x + i) * 0.1,
                                          (cz * CHUNK_SIZE + z + k) * 0.1) + 1) * 10);
            int r = rand() % 1000 + 1;
            int t = CubeType::NONE;
            if (r % 7 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = CubeType::HIGHGRASS;
                t = CubeType::HIGHGRASS;
            } else if (r % 16 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = CubeType::FLOWER_1;
                t = CubeType::FLOWER_1;
            } else if (r % 20 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = CubeType::FLOWER_2;
                t = CubeType::FLOWER_2;
            } else if (r % 20 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = CubeType::FLOWER_3;
                t = CubeType::FLOWER_3;
            } else if (r % 20 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = CubeType::FLOWER_4;
                t = CubeType::FLOWER_4;
            } else if (r % 24 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = CubeType::FLOWER_5;
                t = CubeType::FLOWER_5;
            } else if (r % 24 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = CubeType::FLOWER_6;
                t = CubeType::FLOWER_6;
            }
            if (t != CubeType::NONE) {
                std::string tmpQuery = "(" + std::to_string(cacheMap[cx][cz]) + ", " +
                                       std::to_string(i) + ", " +
                                       std::to_string(h + 1) + ", " +
                                       std::to_string(k) + ", " +
                                       std::to_string(t) + ") ";
                querys.push_back(tmpQuery);
            }
        }
    }
    std::string insertQuery =
            "replace into block" + std::to_string(cacheMap[cx][cz]) + " values ";
    int i = 0;
    for (i = 0; i < querys.size() - 1; i++) {
        insertQuery += querys[i] + ",";
    }
    insertQuery += querys[i] + ";";
    db->execSQL(insertQuery);

}


void MapManager::loadFlower(int cx, int cz) {
    std::string createQuery =
            "create table if not exists block" + std::to_string(cacheMap[cx][cz]) +
            "("
            "    chunkID UNSIGNED BIG INT not null,"
            "    x int not null,"
            "    y int not null,"
            "    z int not null,"
            "    blockType int not null,"
            "    primary key(x, y, z)"
            ");";
    db->execSQL(createQuery);

    Records result;
    std::string q = "select * from block" + std::to_string(cacheMap[cx][cz]);
    db->execSQL(q, result);
    if (result.empty()) {
        std::clog << "gen" << std::endl;
        genFlower(cx, cz);
    } else {
        std::clog << "read" << std::endl;
        for (const auto &row : result) {
            if (row.empty()) break;
            int x = stoi(row[1]), y = stoi(row[2]), z = stoi(row[3]);
            int t = stoi(row[4]);
            if (t >= CubeType::HIGHGRASS && t <= CubeType::FLOWER_6) {
                (*cache)[cx][cz][x][y][z] = t;
            }
        }
    }
}

void MapManager::genFlower(int cx, int cz) {
    int x, z;
    std::tie(x, z) = getChunkVertex(p[0], p[2]);
    x = (x - 1) * 16;
    z = (z - 1) * 16;

    std::vector<std::string> querys;

    for (int i = 0; i < 16; ++i) {
        for (int k = 0; k < 16; ++k) {
            int h = (int) ((n.PerlinNoise((cx * 16 + x + i) * 0.1,
                                          (cz * 16 + z + k) * 0.1) + 1) * 10);
            int r = rand() % 1000 + 1;
            int t = BlockType::NONE;
            if (r % 7 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = BlockType::HIGHGRASS;
                t = BlockType::HIGHGRASS;
            } else if (r % 16 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = BlockType::FLOWER_1;
                t = BlockType::FLOWER_1;
            } else if (r % 20 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = BlockType::FLOWER_2;
                t = BlockType::FLOWER_2;
            } else if (r % 20 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = BlockType::FLOWER_3;
                t = BlockType::FLOWER_3;
            } else if (r % 20 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = BlockType::FLOWER_4;
                t = BlockType::FLOWER_4;
            } else if (r % 24 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = BlockType::FLOWER_5;
                t = BlockType::FLOWER_5;
            } else if (r % 24 == 0) {
                (*cache)[cx][cz][i][h + 1][k] = BlockType::FLOWER_6;
                t = BlockType::FLOWER_6;
            }
            if (t != BlockType::NONE) {
                std::string tmpQuery = "(" + std::to_string(cacheMap[cx][cz]) + ", " +
                                       std::to_string(i) + ", " +
                                       std::to_string(h + 1) + ", " +
                                       std::to_string(k) + ", " +
                                       std::to_string(t) + ") ";
                querys.push_back(tmpQuery);
            }
        }
    }
    std::string insertQuery =
            "replace into block" + std::to_string(cacheMap[cx][cz]) + " values ";
    int i = 0;
    for (i = 0; i < querys.size() - 1; i++) {
        insertQuery += querys[i] + ",";
    }
    insertQuery += querys[i] + ";";
    db->execSQL(insertQuery);

}


void MapManager::loadFlower(int cx, int cz) {
    std::string createQuery =
            "create table if not exists block" + std::to_string(cacheMap[cx][cz]) +
            "("
            "    chunkID UNSIGNED BIG INT not null,"
            "    x int not null,"
            "    y int not null,"
            "    z int not null,"
            "    blockType int not null,"
            "    primary key(x, y, z)"
            ");";
    db->execSQL(createQuery);

    Records result;
    std::string q = "select * from block" + std::to_string(cacheMap[cx][cz]);
    db->execSQL(q, result);
    if (result.empty()) {
        std::clog << "gen" << std::endl;
        genFlower(cx, cz);
    } else {
        std::clog << "read" << std::endl;
        for (const auto &row : result) {
            if (row.empty()) break;
            int x = stoi(row[1]), y = stoi(row[2]), z = stoi(row[3]);
            int t = stoi(row[4]);
            if (t >= BlockType::HIGHGRASS && t <= BlockType::FLOWER_6) {
                (*cache)[cx][cz][x][y][z] = t;
            }
        }
    }
}

std::pair<int32_t, int32_t> getChunkVertex(int32_t x, int32_t z) {
    if (x < 0) {
        if (x % CHUNK_SIZE == 0) {
            x = -(-x) / CHUNK_SIZE;
        } else {
            x = -(-x) / CHUNK_SIZE - 1;
        }
    } else {
        x /= CHUNK_SIZE;
    }
    if (z < 0) {
        if (z % CHUNK_SIZE == 0) {
            z = -(-z) / CHUNK_SIZE;
        } else {
            z = -(-z) / CHUNK_SIZE - 1;
        }
    } else {
        z /= CHUNK_SIZE;
    }
    return std::make_pair(x, z);
}

uint64_t getID(std::pair<int32_t, int32_t> p) {
    uint64_t t = (uint64_t) p.first << 32;
    t += (uint64_t) p.second;
    return t;
}

uint64_t getID(int x, int z) {
    return getID(getChunkVertex(x, z));
}
