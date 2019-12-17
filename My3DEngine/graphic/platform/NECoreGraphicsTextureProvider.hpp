//
//  NECoreGraphicsTextureProvider.hpp
//  NEDemo_Mac
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/11.
//  Copyright © 2019 Liang,Zhiyuan(GIS). All rights reserved.
//

#ifndef NECoreGraphicsTextureProvider_hpp
#define NECoreGraphicsTextureProvider_hpp

#include <stdio.h>
#include "NEMesh.h"
#include "NETextureProvider.hpp"
#include <vector>

typedef unsigned char * NEImageBufferPointer;

class NECoreGraphicsTextureProvider:public  NETextureProvider{
    std::vector<NETexture> _textures;
    NEImageBufferPointer *_imageBuf;
public:
    NECoreGraphicsTextureProvider(std::vector<NETexture> &textures):_textures(textures){
        _imageBuf = (NEImageBufferPointer *)malloc(sizeof(NEImageBufferPointer) * textures.size());
        
        for (int i = 0; i < textures.size(); i++) {
            _imageBuf[i] = nullptr;
        }
    }
    
    virtual bool getTexture(int texIndex, NETexture & tex){
        //todo add range check
        tex = _textures[texIndex];
        return true;
    }
    
    virtual long readColorFromTexture(int textureIndex, float u, float v);
    
    void release(int textureIndex) {};
    void releaseAll() {};
};

#endif /* NECoreGraphicsTextureProvider_hpp */
