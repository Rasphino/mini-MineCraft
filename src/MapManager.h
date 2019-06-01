//
// Created by rasp on 19-5-29.
//

#ifndef EX0_MAPMANAGER_H
#define EX0_MAPMANAGER_H

#include <array>
#include <glm/glm.hpp>

#include "noise.h"

std::pair<int32_t, int32_t> getChunkVertex(int32_t x, int32_t z);
uint64_t getID(std::pair<int32_t, int32_t> p);
uint64_t getID(int x, int z);


// chunk: x,y,z: 16*64*16
using Chunk = int[16][64][16];
using Cache = Chunk[3][3];

class MapManager {
public:
    enum BlockType {
        NONE = -1, SOIL, GRASS, HIGHGRASS, FLOWER_1,
        FLOWER_2, FLOWER_3, FLOWER_4, FLOWER_5, FLOWER_6
    };
    MapManager();
    MapManager(glm::vec3 &pos);
    ~MapManager();
    void genCacheMap(glm::vec3 &pos);
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

};


#endif //EX0_MAPMANAGER_H
