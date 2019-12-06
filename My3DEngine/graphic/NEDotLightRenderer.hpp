//
//  NEDotLightRenderer.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEDotLightRenderer_hpp
#define NEDotLightRenderer_hpp

#include <stdio.h>
#include "NEStandardRenderer.hpp"

class NEDotLightRenderer : public NEStandardRenderer {
    
    
public:
    NEDotLightRenderer(int width, int height):NEStandardRenderer(width,height){
        NEFrustum fru;
        fru.r = 0.2;
        fru.t = 0.2;
        
        fru.near = 0.2;
        fru.far = 100;
        
        camera.frustum = fru;
    }
    
    void clear(){_depthBuffer.resetSize();_renderBufferSize = 0;}
    
    //1 light touchable
    //0 light untouchable
    int testPosition_world(NEVector3 & worldPos);
};

#endif /* NEDotLightRenderer_hpp */
