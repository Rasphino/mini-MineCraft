//
//  TextureAtlas.h
//  shader
//
//  Created by jones on 2019/5/2.
//  Copyright © 2019 jones. All rights reserved.
//

#ifndef TextureAtlas_h
#define TextureAtlas_h
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <iostream>
#include <vector>
#include <string>
#include "stb_image.h"
#include <glm/glm.hpp>
using namespace std;

class TextureAtlas : public Texture
{
public:
    TextureAtlas(string& textureFileName) {
        LoadFile(textureFileName);
        
        m_imageSize             = 256;
        m_individualTextureSize = 16;
    }
    vector<GLfloat> getTexture(const glm::vec2& coords) {
        static const GLfloat TEX_PER_ROW      = (GLfloat)m_imageSize / (GLfloat)m_individualTextureSize;
        static const GLfloat INDV_TEX_SIZE    = 1.0f / TEX_PER_ROW;
        static const GLfloat PIXEL_SIZE       = 1.0f / (float)m_imageSize;
        
        GLfloat xMin = (coords.x * INDV_TEX_SIZE) + 0.5 * PIXEL_SIZE;
        GLfloat yMin = (coords.y * INDV_TEX_SIZE) + 0.5 * PIXEL_SIZE;
        
        GLfloat xMax = (xMin + INDV_TEX_SIZE) - 0.5 * PIXEL_SIZE;
        GLfloat yMax = (yMin + INDV_TEX_SIZE) - 0.5 * PIXEL_SIZE;
        
        return
        {
            xMax, yMax,
            xMin, yMax,
            xMin, yMin,
            xMax, yMin
        };
    }
    
private:
    int m_imageSize;
    int m_individualTextureSize;
};

#endif // TEXTUREATLAS_H_INCLUDED
