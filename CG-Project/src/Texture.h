//
//  Texture.h
//  shader
//
//  Created by jones on 2019/4/25.
//  Copyright © 2019 jones. All rights reserved.
//

#ifndef Texture_h
#define Texture_h
#include <string>

#include "stb_image.h"

class Texture {
public:
    unsigned int ID;
    unsigned char* data;
    int width;
    int height;
    int nrChannels;
    //int texType;
    Texture() = default;
    Texture(std::string& filename) {
        LoadFile(filename);
    }
    void LoadFile(std::string& filename) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
    }
    void BindTexture() const {
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    void TexActivate() const {
        glActiveTexture(GL_TEXTURE0);
    }
    ~Texture() {
        glDeleteTextures(1, &ID);
    }
};

#endif /* Texture_h */
