//
//  NECoreGraphicsDevice.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NECoreGraphicsDevice_hpp
#define NECoreGraphicsDevice_hpp

#include <stdio.h>
#include "NECommon.h"
#include "NEDisplayDevice.hpp"

class NECoreGraphicsDevice : public NEDisplayDevice {
    
public:
    virtual void presentRenderBuffer(RenderBufferType *renderBuffer, int renderSize, int mode, void *extra);
    
    virtual void clearDevice(NERect clearRect);
};

#endif /* NECoreGraphicsDevice_hpp */
