//
// Created by rasp on 19-5-29.
//

#ifndef EX0_MAPMANAGER_H
#define EX0_MAPMANAGER_H

#include <array>
#include <tuple>

#include <glm/glm.hpp>
#include <thread>

#include "Cube.h"
#include "MCdb.h"
#include "noise.h"

const int CHUNK_SIZE = 16;
const int CHUNK_NUM = 3;

// 获取（x,z) 所在chunk的顶点坐标
std::pair<int32_t, int32_t> getChunkVertex(int32_t x, int32_t z);
// 输入chunk的顶点坐标，获取chunk ID
uint64_t getID(std::pair<int32_t, int32_t> p);
uint64_t getID(int x, int z);

using Chunk = int[CHUNK_SIZE][64][CHUNK_SIZE];
using Cache = Chunk[CHUNK_NUM][CHUNK_NUM];

class MapManager {
public:
    MapManager();
    MapManager(glm::vec3& pos);
    ~MapManager();

    void updateCacheMap(glm::vec3& pos);
    void genCacheFromNoise();
    Cache *getCache();
    std::pair<int32_t, int32_t> getCacheVertexCoord();

private:
    // 缓存周围3x3个chunk的数据
    Cache* cache;
    std::array<std::array<uint64_t, CHUNK_NUM>, CHUNK_NUM> cacheMap;
    std::vector<std::tuple<int, int, int, int> > deltaList[CHUNK_NUM][CHUNK_NUM];
    glm::vec3 p;
    Perlin n;
    MCdb* db;

    void genCacheMap(glm::vec3 &pos);
    void loadDeltaBlock(int cx, int cz);
    void genFlower(int cx, int cz);
    void writeBack();
};

#endif // EX0_MAPMANAGER_H