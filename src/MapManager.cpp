//
// Created by rasp on 19-5-29.
//

#include <utility>
#include <tuple>
#include "MapManager.h"

MapManager::MapManager() {
    cache = new Cache[3];
}

MapManager::MapManager(glm::vec3 &pos) {
    cache = new Cache[3];
    genCacheMap(pos);
    p = pos;
    genCacheFromNoise();
}

MapManager::~MapManager() {
    delete[] cache;
}

void MapManager::genCacheMap(glm::vec3 &pos) {
    cacheMap[0][0] = getID(int(pos[0] - 16), int(pos[2] - 16));
    cacheMap[0][1] = getID(int(pos[0] - 16), int(pos[2]));
    cacheMap[0][2] = getID(int(pos[0] - 16), int(pos[2] + 16));
    cacheMap[1][0] = getID(int(pos[0]), int(pos[2] - 16));
    cacheMap[1][1] = getID(int(pos[0]), int(pos[2]));
    cacheMap[1][2] = getID(int(pos[0]), int(pos[2] + 16));
    cacheMap[2][0] = getID(int(pos[0] - 16), int(pos[2] + 16));
    cacheMap[2][1] = getID(int(pos[0]), int(pos[2] + 16));
    cacheMap[2][2] = getID(int(pos[0] + 16), int(pos[2] + 16));
}

void MapManager::updateCacheMap(glm::vec3 &pos) {
    // if pos and lastPos are in the same chunk, no need to update cache map
    if (getID(int(p[0]), int(p[2])) == getID(int(pos[0]), int(pos[2]))) {
        return;
    }

    genCacheMap(pos);
    p = pos;
    genCacheFromNoise();
}

void MapManager::genCacheFromNoise() {
    int x, z;
    std::tie(x, z) = getChunkVertex(p[0], p[2]);
    x = (x - 1) * 16;
    z = (z - 1) * 16;

    for (int cx = 0; cx < 3; ++cx) {
        for (int cz = 0; cz < 3; ++cz) {
            for (int i = 0; i < 16; ++i) {
                for (int k = 0; k < 16; ++k) {
                    int h = (int) (
                            (n.PerlinNoise((cx * 16 + x + i) * 0.1, (cz * 16 + z + k) * 0.1) +
                             1) *
                            10);
                    for (int j = 0; j < h; ++j) {
                        (*cache)[cx][cz][i][j][k] = BlockType::SOIL;
                    }
                    for (int j = h; j < 64; ++j) {
                        (*cache)[cx][cz][i][j][k] = BlockType::NONE;
                    }
                    (*cache)[cx][cz][i][h][k] = BlockType::GRASS;
                }
            }
        }
    }

}

Cache *MapManager::getCache() {
    return cache;
}

std::pair<int32_t, int32_t> MapManager::getCacheVertexCoord() {
    int x, z;
    std::tie(x, z) = getChunkVertex(p[0], p[2]);
    x--;
    z--;
    return std::pair<int32_t, int32_t>(x * 16, z * 16);
}

std::pair<int32_t, int32_t> getChunkVertex(int32_t x, int32_t z) {
    if (x < 0) {
        if (x % 16 == 0) {
            x = -(-x) / 16;
        } else {
            x = -(-x) / 16 - 1;
        }
    } else {
        x /= 16;
    }
    if (z < 0) {
        if (z % 16 == 0) {
            z = -(-z) / 16;
        } else {
            z = -(-z) / 16 - 1;
        }
    } else {
        z /= 16;
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

