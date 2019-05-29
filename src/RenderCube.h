//
//  RenderCube.h
//  shader
//
//  Created by jones on 2019/5/2.
//  Copyright © 2019 jones. All rights reserved.
//

#ifndef RenderCube_h
#define RenderCube_h

typedef struct _VertexData {
    float position[3];
    float texture[2];
}VertexData;

class CubeRender {
private:
    VertexData[36];
    
public:
    CubeRender(string type) {
        VertexData[ 0].position = {-0.5f, -0.5f, -0.5f};
        VertexData[ 1].position = { 0.5f, -0.5f, -0.5f};
        VertexData[ 2].position = { 0.5f,  0.5f, -0.5f};
        VertexData[ 3].position = { 0.5f,  0.5f, -0.5f};
        VertexData[ 4].position = {-0.5f,  0.5f, -0.5f};
        VertexData[ 5].position = {-0.5f, -0.5f, -0.5f};
        
        VertexData[ 6].position = {-0.5f, -0.5f,  0.5f};
        VertexData[ 7].position = { 0.5f, -0.5f,  0.5f};
        VertexData[ 8].position = { 0.5f,  0.5f,  0.5f};
        VertexData[ 9].position = { 0.5f,  0.5f,  0.5f};
        VertexData[10].position = {-0.5f,  0.5f,  0.5f};
        VertexData[11].position = {-0.5f, -0.5f,  0.5f};
        //左面
        VertexData[12].position = {-0.5f, -0.5f, -0.5f};
        VertexData[13].position = {-0.5f, -0.5f,  0.5f};
        VertexData[14].position = {-0.5f,  0.5f,  0.5f};
        VertexData[15].position = {-0.5f,  0.5f,  0.5f};
        VertexData[16].position = {-0.5f,  0.5f, -0.5f};
        VertexData[17].position = {-0.5f, -0.5f, -0.5f};
        //右面
        
        VertexData[18].position = { 0.5f, -0.5f, -0.5f};
        VertexData[19].position = { 0.5f, -0.5f,  0.5f};
        VertexData[20].position = { 0.5f,  0.5f,  0.5f};
        VertexData[21].position = { 0.5f,  0.5f,  0.5f};
        VertexData[22].position = { 0.5f,  0.5f, -0.5f};
        VertexData[23].position = { 0.5f, -0.5f, -0.5f};
        //下面
        VertexData[24].position = {-0.5f, -0.5f, -0.5f};
        VertexData[25].position = { 0.5f, -0.5f, -0.5f};
        VertexData[26].position = { 0.5f, -0.5f,  0.5f};
        VertexData[27].position = { 0.5f, -0.5f,  0.5f};
        VertexData[28].position = {-0.5f, -0.5f,  0.5f};
        VertexData[29].position = {-0.5f, -0.5f, -0.5f};
        //上面
        
        VertexData[30].position = {-0.5f,  0.5f, -0.5f};
        VertexData[31].position = { 0.5f,  0.5f, -0.5f};
        VertexData[32].position = { 0.5f,  0.5f,  0.5f};
        VertexData[33].position = { 0.5f,  0.5f,  0.5f};
        VertexData[34].position = {-0.5f,  0.5f,  0.5f};
        VertexData[35].position = {-0.5f,  0.5f, -0.5f};
        
        if(type == "grass") {
            
        }
    }
    
}
#endif /* RenderCube_h */
