//
//  NEDotLightRenderer.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEDotLightRenderer.hpp"

int NEDotLightRenderer::testPosition_world(NEVector3 & worldPos){
    NEVector3 vt = convertToEyeSpace(worldPos);
    NEVector2 vInView = pointInVewForVector3(vt);
    
    DepthInfo info = _depthBuffer.getInfo(vInView.x, vInView.y);
    if (vt.z <= info.z) {
        return 1;
    }
    
    return 0;
}
