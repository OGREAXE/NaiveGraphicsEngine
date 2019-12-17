//
//  NETextureProvider.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/11.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NETextureProvider_hpp
#define NETextureProvider_hpp

#include <stdio.h>
#include "NEMesh.h"

class NETextureProvider {
public:
    virtual long readColorFromTexture(int textureIndex, float u, float v) = 0;
    
    virtual void release(int textureIndex) = 0;
    virtual void releaseAll() = 0;
    
    virtual bool getTexture(int texIndex, NETexture & tex) = 0;
};

#endif /* NETextureProvider_hpp */
