#ifndef _SKY_H
#define _SKY_H

#include <GL/glut.h>

class Sky {
public:
    Sky();
    //初始化天空的位置，半径和纹理
    void InitSky(float skyX, float skyY, float skyZ, float skyR, unsigned int texture);
    //显示天空
    void ShowSky(void);
    //获得天空的属性
    float GetSkyX(void);
    float GetSkyY(void);
    float GetSkyZ(void);
    float GetSkyR(void);

public:
    float X;
    float Y;
    float Z;
    float R;
    unsigned int T;
    GLUquadricObj *qobj;
};

#endif
