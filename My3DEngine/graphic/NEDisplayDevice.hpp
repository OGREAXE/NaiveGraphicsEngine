//
//  NEDisplayDevice.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEDisplayDevice_hpp
#define NEDisplayDevice_hpp
#include "NECommon.h"

#include <stdio.h>

#define RenderValueGetX(renderValue) (renderValue & 0xffff)
#define RenderValueGetY(renderValue) ((renderValue >> 16) & 0xffff)
#define RenderValueGetColor(renderValue) (renderValue >> 32);

class NEDisplayDevice {
    
    
public:
    virtual void presentRenderBuffer(RenderBufferType *renderBuffer, int renderSize, int mode, void *extra) = 0;
};

#endif /* NEDisplayDevice_hpp */
