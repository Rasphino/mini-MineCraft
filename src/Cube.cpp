#include "Cube.h"
#include<iostream>
#include<cstdlib>
#include <cstring>
#include "stb_image.h"

using namespace std;
// settings
#define SCR_WIDTH = 800;
#define SCR_HEIGHT = 600;


typedef struct _VertexData {
    float position[3];
    float texture[2];
} VertexData;

Cube::Cube() : grassShader("shaders/grass.vs", "shaders/grass.fs"),
               soilShader("shaders/soil.vs", "shaders/soil.fs"),
               transparentShader("shaders/transparent.vs", "shaders/transparent.fs") {
    const float size = 1 / 16.0;
    VertexData grassCube[] = {
            //后面
            {{0.0f, 0.0f, 0.0f},
                    {0,    size}},
            {{1.0f, 0.0f, 0.0f},
                    {size, size}},
            {{1.0f, 1.0f, 0.0f},
                    {size, 2 * size}},
            {{1.0f, 1.0f, 0.0f},
                    {size, 2 * size}},
            {{0.0f, 1.0f, 0.0f},
                    {0,    2 * size}},
            {{0.0f, 0.0f, 0.0f},
                    {0,    size}},

            //前面
            {{0.0f, 0.0f, 1.0f},
                    {0,    size}},
            {{1.0f, 0.0f, 1.0f},
                    {size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, 2 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, 2 * size}},
            {{0.0f, 1.0f, 1.0f},
                    {0,    2 * size}},
            {{0.0f, 0.0f, 1.0f},
                    {0,    size}},

            //左面
            {{0.0f, 0.0f, 0.0f},
                    {0,    size}},
            {{0.0f, 0.0f, 1.0f},
                    {size, size}},
            {{0.0f, 1.0f, 1.0f},
                    {size, 2 * size}},
            {{0.0f, 1.0f, 1.0f},
                    {size, 2 * size}},
            {{0.0f, 1.0f, 0.0f},
                    {0,    2 * size}},
            {{0.0f, 0.0f, 0.0f},
                    {0,    size}},

            //右面
            {{1.0f, 0.0f, 0.0f},
                    {0,    size}},
            {{1.0f, 0.0f, 1.0f},
                    {size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, 2 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, 2 * size}},
            {{1.0f, 1.0f, 0.0f},
                    {0,    2 * size}},
            {{1.0f, 0.0f, 0.0f},
                    {0,    size}},

            //下面
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},
            {{1.0f, 0.0f, 0.0f},
                    {size, 0}},
            {{1.0f, 0.0f, 1.0f},
                    {size, size}},
            {{1.0f, 0.0f, 1.0f},
                    {size, size}},
            {{0.0f, 0.0f, 1.0f},
                    {0,    size}},
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},

            //上面

            {{0.0f, 1.0f, 0.0f},
                    {0,    2 * size}},
            {{1.0f, 1.0f, 0.0f},
                    {size, 2 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, 3 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, 3 * size}},
            {{0.0f, 1.0f, 1.0f},
                    {0,    3 * size}},
            {{0.0f, 1.0f, 0.0f},
                    {0,    2 * size}},
    };

    VertexData soilCube[] = {
            //后面
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},
            {{1.0f, 0.0f, 0.0f},
                    {size, 0}},
            {{1.0f, 1.0f, 0.0f},
                    {size, size}},
            {{1.0f, 1.0f, 0.0f},
                    {size, size}},
            {{0.0f, 1.0f, 0.0f},
                    {0,    size}},
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},

            //前面
            {{0.0f, 0.0f, 1.0f},
                    {0,    0}},
            {{1.0f, 0.0f, 1.0f},
                    {size, 0}},
            {{1.0f, 1.0f, 1.0f},
                    {size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, size}},
            {{0.0f, 1.0f, 1.0f},
                    {0,    size}},
            {{0.0f, 0.0f, 1.0f},
                    {0,    0}},
            //左面
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},
            {{0.0f, 0.0f, 1.0f},
                    {size, 0}},
            {{0.0f, 1.0f, 1.0f},
                    {size, size}},
            {{0.0f, 1.0f, 1.0f},
                    {size, size}},
            {{0.0f, 1.0f, 0.0f},
                    {0,    size}},
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},
            //右面
            {{1.0f, 0.0f, 0.0f},
                    {0,    0}},
            {{1.0f, 0.0f, 1.0f},
                    {size, 0}},
            {{1.0f, 1.0f, 1.0f},
                    {size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, size}},
            {{1.0f, 1.0f, 0.0f},
                    {0,    size}},
            {{1.0f, 0.0f, 0.0f},
                    {0,    0}},
            //下面
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},
            {{1.0f, 0.0f, 0.0f},
                    {size, 0}},
            {{1.0f, 0.0f, 1.0f},
                    {size, size}},
            {{1.0f, 0.0f, 1.0f},
                    {size, size}},
            {{0.0f, 0.0f, 1.0f},
                    {0,    size}},
            {{0.0f, 0.0f, 0.0f},
                    {0,    0}},

            //上面

            {{0.0f, 1.0f, 0.0f},
                    {0,    0}},
            {{1.0f, 1.0f, 0.0f},
                    {size, 0}},
            {{1.0f, 1.0f, 1.0f},
                    {size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {size, size}},
            {{0.0f, 1.0f, 1.0f},
                    {0,    size}},
            {{0.0f, 1.0f, 0.0f},
                    {0,    0}},
    };
    VertexData rockCube[] = {
            //后面
            {{0.0f, 0.0f, 0.0f},
                    {5 * size, 0}},
            {{1.0f, 0.0f, 0.0f},
                    {6 * size, 0}},
            {{1.0f, 1.0f, 0.0f},
                    {6 * size, size}},
            {{1.0f, 1.0f, 0.0f},
                    {6 * size, size}},
            {{0.0f, 1.0f, 0.0f},
                    {5 * size, size}},
            {{0.0f, 0.0f, 0.0f},
                    {5 * size, 0}},

            //前面
            {{0.0f, 0.0f, 1.0f},
                    {5 * size, 0}},
            {{1.0f, 0.0f, 1.0f},
                    {6 * size, 0}},
            {{1.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{0.0f, 1.0f, 1.0f},
                    {5 * size, size}},
            {{0.0f, 0.0f, 1.0f},
                    {5 * size, 0}},

            //左面
            {{0.0f, 0.0f, 0.0f},
                    {5 * size, 0}},
            {{0.0f, 0.0f, 1.0f},
                    {6 * size, 0}},
            {{0.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{0.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{0.0f, 1.0f, 0.0f},
                    {5 * size, size}},
            {{0.0f, 0.0f, 0.0f},
                    {5 * size, 0}},

            //右面
            {{1.0f, 0.0f, 0.0f},
                    {5 * size, 0}},
            {{1.0f, 0.0f, 1.0f},
                    {6 * size, 0}},
            {{1.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{1.0f, 1.0f, 0.0f},
                    {5 * size, size}},
            {{1.0f, 0.0f, 0.0f},
                    {5 * size, 0}},

            //下面
            {{0.0f, 0.0f, 0.0f},
                    {5 * size, 0}},
            {{1.0f, 0.0f, 0.0f},
                    {6 * size, 0}},
            {{1.0f, 0.0f, 1.0f},
                    {6 * size, size}},
            {{1.0f, 0.0f, 1.0f},
                    {6 * size, size}},
            {{0.0f, 0.0f, 1.0f},
                    {5 * size, size}},
            {{0.0f, 0.0f, 0.0f},
                    {5 * size, 0}},

            //上面
            {{0.0f, 1.0f, 0.0f},
                    {5 * size, 0}},
            {{1.0f, 1.0f, 0.0f},
                    {6 * size, 0}},
            {{1.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{1.0f, 1.0f, 1.0f},
                    {6 * size, size}},
            {{0.0f, 1.0f, 1.0f},
                    {5 * size, size}},
            {{0.0f, 1.0f, 0.0f},
                    {5 * size, 0}},
    };
    VertexData highGrassCube[] = {
            //后面
            {
                    {0.0f, 0.0f, 1.0f},
                    {0,    3 * size}
            },
            {
                    {1.0f, 0.0f, 0.0f},
                    {size, 3 * size}
            },
            {
                    {1.0f, 1.0f, 0.0f},
                    {size, 4 * size}
            },
            {
                    {1.0f, 1.0f, 0.0f},
                    {size, 4 * size}
            },
            {
                    {0.0f, 1.0f, 1.0f},
                    {0,    4 * size}
            },
            {
                    {0.0f, 0.0f, 1.0f},
                    {0,    3 * size}
            },

            //前面
            {
                    {0.0f, 0.0f, 0.0f},
                    {0,    3 * size}
            },
            {
                    {1.0f, 0.0f, 1.0f},
                    {size, 3 * size}
            },
            {
                    {1.0f, 1.0f, 1.0f},
                    {size, 4 * size}
            },
            {
                    {1.0f, 1.0f, 1.0f},
                    {size, 4 * size}
            },
            {
                    {0.0f, 1.0f, 0.0f},
                    {0,    4 * size}
            },
            {
                    {0.0f, 0.0f, 0.0f},
                    {0,    3 * size}
            },

    };
    VertexData flower_1Cube[] = {
            //后面
            {
                    {-0.35f, 0.0f, 0.35f},
                    {size,     3 * size}
            },
            {
                    {0.35f,  0.0f, -0.35f},
                    {2 * size, 3 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {2 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {2 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, 0.35f},
                    {size,     4 * size}
            },
            {
                    {-0.35f, 0.0f, 0.35f},
                    {size,     3 * size}
            },

            //前面
            {
                    {0.0f,   0.0f, 0.0f},
                    {size,     3 * size}
            },
            {
                    {1.0f,   0.0f, 1.0f},
                    {2 * size, 3 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {2 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {2 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 0.0f},
                    {size,     4 * size}
            },
            {
                    {0.0f,   0.0f, 0.0f},
                    {size,     3 * size}
            },

    };
    VertexData flower_2Cube[] = {
            //后面
            {
                    {-0.35f, 0.0f, 0.35f},
                    {2 * size, 3 * size}
            },
            {
                    {0.35f,  0.0f, -0.35f},
                    {3 * size, 3 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {3 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {3 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, 0.35f},
                    {2 * size, 4 * size}
            },
            {
                    {-0.35f, 0.0f, 0.35f},
                    {2 * size, 3 * size}
            },

            //前面
            {
                    {0.0f,   0.0f, 0.0f},
                    {2 * size, 3 * size}
            },
            {
                    {1.0f,   0.0f, 1.0f},
                    {3 * size, 3 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {3 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {3 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 0.0f},
                    {2 * size, 4 * size}
            },
            {
                    {0.0f,   0.0f, 0.0f},
                    {2 * size, 3 * size}
            },

    };
    VertexData flower_3Cube[] = {
            //后面
            {
                    {-0.35f, 0.0f, 0.35f},
                    {3 * size, 3 * size}
            },
            {
                    {0.35f,  0.0f, -0.35f},
                    {4 * size, 3 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {4 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {4 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, 0.35f},
                    {3 * size, 4 * size}
            },
            {
                    {-0.35f, 0.0f, 0.35f},
                    {3 * size, 3 * size}
            },

            //前面
            {
                    {0.0f,   0.0f, 0.0f},
                    {3 * size, 3 * size}
            },
            {
                    {1.0f,   0.0f, 1.0f},
                    {4 * size, 3 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {4 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {4 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 0.0f},
                    {3 * size, 4 * size}
            },
            {
                    {0.0f,   0.0f, 0.0f},
                    {3 * size, 3 * size}
            },

    };
    VertexData flower_4Cube[] = {
            //后面
            {
                    {-0.35f, 0.0f, 0.35f},
                    {4 * size, 3 * size}
            },
            {
                    {0.35f,  0.0f, -0.35f},
                    {5 * size, 3 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {5 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {5 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, 0.35f},
                    {4 * size, 4 * size}
            },
            {
                    {-0.35f, 0.0f, 0.35f},
                    {4 * size, 3 * size}
            },

            //前面
            {
                    {0.0f,   0.0f, 0.0f},
                    {4 * size, 3 * size}
            },
            {
                    {1.0f,   0.0f, 1.0f},
                    {5 * size, 3 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {5 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {5 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 0.0f},
                    {4 * size, 4 * size}
            },
            {
                    {0.0f,   0.0f, 0.0f},
                    {4 * size, 3 * size}
            },

    };
    VertexData flower_5Cube[] = {
            {
                    {0.35f,  0.0f, 0.35f},
                    {5 * size, 3 * size}
            },
            {
                    {-0.35f, 0.0f, -0.35f},
                    {6 * size, 3 * size}
            },
            {
                    {-0.35f, 1.0f, -0.35f},
                    {6 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, -0.35f},
                    {6 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, 0.35f},
                    {5 * size, 4 * size}
            },
            {
                    {0.35f,  0.0f, 0.35f},
                    {5 * size, 3 * size}
            },

            //前面
            {
                    {1.0f,   0.0f, 0.0f},
                    {5 * size, 3 * size}
            },
            {
                    {0.0f,   0.0f, 1.0f},
                    {6 * size, 3 * size}
            },
            {
                    {0.0f,   1.0f, 1.0f},
                    {6 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 1.0f},
                    {6 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 0.0f},
                    {5 * size, 4 * size}
            },
            {
                    {1.0f,   0.0f, 0.0f},
                    {5 * size, 3 * size}
            },

            {
                    {-0.35f, 0.0f, 0.35f},
                    {5 * size, 3 * size}
            },
            {
                    {0.35f,  0.0f, -0.35f},
                    {6 * size, 3 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {6 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {6 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, 0.35f},
                    {5 * size, 4 * size}
            },
            {
                    {-0.35f, 0.0f, 0.35f},
                    {5 * size, 3 * size}
            },

            //前面
            {
                    {0.0f,   0.0f, 0.0f},
                    {5 * size, 3 * size}
            },
            {
                    {1.0f,   0.0f, 1.0f},
                    {6 * size, 3 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {6 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {6 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 0.0f},
                    {5 * size, 4 * size}
            },
            {
                    {0.0f,   0.0f, 0.0f},
                    {5 * size, 3 * size}
            },

    };
    VertexData flower_6Cube[] = {

            {
                    {0.35f,  0.0f, 0.35f},
                    {6 * size, 3 * size}
            },
            {
                    {-0.35f, 0.0f, -0.35f},
                    {7 * size, 3 * size}
            },
            {
                    {-0.35f, 1.0f, -0.35f},
                    {7 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, -0.35f},
                    {7 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, 0.35f},
                    {6 * size, 4 * size}
            },
            {
                    {0.35f,  0.0f, 0.35f},
                    {6 * size, 3 * size}
            },

            //前面
            {
                    {1.0f,   0.0f, 0.0f},
                    {6 * size, 3 * size}
            },
            {
                    {0.0f,   0.0f, 1.0f},
                    {7 * size, 3 * size}
            },
            {
                    {0.0f,   1.0f, 1.0f},
                    {7 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 1.0f},
                    {7 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 0.0f},
                    {6 * size, 4 * size}
            },
            {
                    {1.0f,   0.0f, 0.0f},
                    {6 * size, 3 * size}
            },

            {
                    {-0.35f, 0.0f, 0.35f},
                    {6 * size, 3 * size}
            },
            {
                    {0.35f,  0.0f, -0.35f},
                    {7 * size, 3 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {7 * size, 4 * size}
            },
            {
                    {0.35f,  1.0f, -0.35f},
                    {7 * size, 4 * size}
            },
            {
                    {-0.35f, 1.0f, 0.35f},
                    {6 * size, 4 * size}
            },
            {
                    {-0.35f, 0.0f, 0.35f},
                    {6 * size, 3 * size}
            },

            //前面
            {
                    {0.0f,   0.0f, 0.0f},
                    {6 * size, 3 * size}
            },
            {
                    {1.0f,   0.0f, 1.0f},
                    {7 * size, 3 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {7 * size, 4 * size}
            },
            {
                    {1.0f,   1.0f, 1.0f},
                    {7 * size, 4 * size}
            },
            {
                    {0.0f,   1.0f, 0.0f},
                    {6 * size, 4 * size}
            },
            {
                    {0.0f,   0.0f, 0.0f},
                    {6 * size, 3 * size}
            },

    };

    glGenVertexArrays(1, &VAO_grass);
    glGenBuffers(1, &VBO_grass);
    glBindVertexArray(VAO_grass);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_grass);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassCube), grassCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(grassCube[0].position)));

    glGenVertexArrays(1, &VAO_soil);
    glGenBuffers(1, &VBO_soil);
    glBindVertexArray(VAO_soil);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_soil);
    glBufferData(GL_ARRAY_BUFFER, sizeof(soilCube), soilCube, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(soilCube[0].position)));

    glGenVertexArrays(1, &VAO_highGrass);
    glGenBuffers(1, &VBO_highGrass);
    glBindVertexArray(VAO_highGrass);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_highGrass);
    glBufferData(GL_ARRAY_BUFFER, sizeof(highGrassCube), highGrassCube, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(highGrassCube[0].position)));

    glGenVertexArrays(1, &VAO_flower_1);
    glGenBuffers(1, &VBO_flower_1);
    glBindVertexArray(VAO_flower_1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flower_1Cube), flower_1Cube, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(flower_1Cube[0].position)));

    glGenVertexArrays(1, &VAO_flower_2);
    glGenBuffers(1, &VBO_flower_2);
    glBindVertexArray(VAO_flower_2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flower_2Cube), flower_2Cube, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(flower_2Cube[0].position)));

    glGenVertexArrays(1, &VAO_flower_3);
    glGenBuffers(1, &VBO_flower_3);
    glBindVertexArray(VAO_flower_3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flower_3Cube), flower_3Cube, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(flower_3Cube[0].position)));

    glGenVertexArrays(1, &VAO_flower_4);
    glGenBuffers(1, &VBO_flower_4);
    glBindVertexArray(VAO_flower_4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flower_4Cube), flower_4Cube, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(flower_4Cube[0].position)));

    glGenVertexArrays(1, &VAO_flower_5);
    glGenBuffers(1, &VBO_flower_5);
    glBindVertexArray(VAO_flower_5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flower_5Cube), flower_5Cube, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(flower_5Cube[0].position)));

    glGenVertexArrays(1, &VAO_flower_6);
    glGenBuffers(1, &VBO_flower_6);
    glBindVertexArray(VAO_flower_6);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_6);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flower_6Cube), flower_6Cube, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(flower_6Cube[0].position)));

}

void Cube::Draw(CubeType name, glm::vec3 pos, glm::mat4 projection, glm::mat4 view,
                glm::vec3 cameraPos) {

    if (name == CubeType::SOIL) {
        glBindVertexArray(VAO_soil);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_soil);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        soilShader.use();
        soilShader.setMat4("projection", projection);
        soilShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(soilShader.ID, "soil"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        soilShader.setMat4("model", model);
        soilShader.setVec3("cubePos", pos);
        soilShader.setVec3("cameraPos", cameraPos);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::GRASS) {
        glBindVertexArray(VAO_grass);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_grass);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        grassShader.use();
        grassShader.setMat4("projection", projection);
        grassShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(grassShader.ID, "grass"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        grassShader.setMat4("model", model);
        grassShader.setVec3("cubePos", pos);
        grassShader.setVec3("cameraPos", cameraPos);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::HIGHGRASS) {
        glBindVertexArray(VAO_highGrass);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_highGrass);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        transparentShader.use();
        transparentShader.setMat4("projection", projection);
        transparentShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(transparentShader.ID, "highGrass"),
                    0);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        transparentShader.setVec3("cubePos", pos);
        transparentShader.setVec3("cameraPos", cameraPos);
        transparentShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::FLOWER_1) {
        glBindVertexArray(VAO_flower_1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_1);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        transparentShader.use();
        transparentShader.setMat4("projection", projection);
        transparentShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(transparentShader.ID, "highGrass"),
                    0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        transparentShader.setVec3("cubePos", pos);
        transparentShader.setVec3("cameraPos", cameraPos);
        transparentShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::FLOWER_2) {
        glBindVertexArray(VAO_flower_2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_2);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        transparentShader.use();
        transparentShader.setMat4("projection", projection);
        transparentShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(transparentShader.ID, "highGrass"),
                    0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        transparentShader.setVec3("cubePos", pos);
        transparentShader.setVec3("cameraPos", cameraPos);
        transparentShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::FLOWER_3) {
        glBindVertexArray(VAO_flower_3);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        transparentShader.use();
        transparentShader.setMat4("projection", projection);
        transparentShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(transparentShader.ID, "highGrass"),
                    0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        transparentShader.setVec3("cubePos", pos);
        transparentShader.setVec3("cameraPos", cameraPos);
        transparentShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::FLOWER_4) {
        glBindVertexArray(VAO_flower_4);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_4);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        transparentShader.use();
        transparentShader.setMat4("projection", projection);
        transparentShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(transparentShader.ID, "highGrass"),
                    0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        transparentShader.setVec3("cubePos", pos);
        transparentShader.setVec3("cameraPos", cameraPos);
        transparentShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::FLOWER_5) {
        glBindVertexArray(VAO_flower_5);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_5);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        transparentShader.use();
        transparentShader.setMat4("projection", projection);
        transparentShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(transparentShader.ID, "highGrass"),
                    0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        transparentShader.setVec3("cubePos", pos);
        transparentShader.setVec3("cameraPos", cameraPos);
        transparentShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } else if (name == CubeType::FLOWER_6) {
        glBindVertexArray(VAO_flower_6);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_flower_6);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        transparentShader.use();
        transparentShader.setMat4("projection", projection);
        transparentShader.setMat4("view", view);
        glUniform1i(glGetUniformLocation(transparentShader.ID, "highGrass"),
                    0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        transparentShader.setVec3("cubePos", pos);
        transparentShader.setVec3("cameraPos", cameraPos);
        transparentShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO_grass);
    glDeleteBuffers(1, &VBO_grass);
    glDeleteVertexArrays(1, &VAO_soil);
    glDeleteBuffers(1, &VBO_soil);
}
