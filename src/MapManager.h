//
// Created by rasp on 19-5-29.
//

#ifndef EX0_MAPMANAGER_H
#define EX0_MAPMANAGER_H

#include <array>
#include <glm/glm.hpp>

#include "noise.h"
#include "MCdb.h"
#include "Cube.h"

// 获取（x,z) 所在chunk的顶点坐标
std::pair<int32_t, int32_t> getChunkVertex(int32_t x, int32_t z);
// 输入chunk的顶点坐标，获取chunk ID
uint64_t getID(std::pair<int32_t, int32_t> p);
uint64_t getID(int x, int z);


// chunk: x,y,z: 16*64*16
using Chunk = int[16][64][16];
// cache: 3*3个chunk
using Cache = Chunk[3][3];

class MapManager {
public:

    MapManager();
    MapManager(glm::vec3 &pos);
    ~MapManager();

    void updateCacheMap(glm::vec3 &pos);
    void genCacheFromNoise();
    Cache *getCache();
    std::pair<int32_t, int32_t> getCacheVertexCoord();

private:
    // 缓存周围3x3个chunk的数据
    Cache *cache;
    std::array<std::array<uint64_t, 3>, 3> cacheMap;
    glm::vec3 p;
    Perlin n;
    MCdb *db;

    void genCacheMap(glm::vec3 &pos);
    void loadFlower(int cx, int cz);
    void genFlower(int cx, int cz);
};


#endif //EX0_MAPMANAGER_H