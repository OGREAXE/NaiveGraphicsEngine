//
//  NEComposedRenderer.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEComposedRenderer_hpp
#define NEComposedRenderer_hpp

#include <stdio.h>
#include "NEStandardRenderer.hpp"
#include <vector>
#include "NEDotLight.hpp"
#include "NECommon.h"
#include "NEDisplayDevice.hpp"

class NEComposedRenderer:NEStandardRenderer{
    NEDotLight *_dotLight0;
    NEVector3 _dotLightPositionInCameraSpace;
    NEDisplayDevice *_device;
    
protected:
    NEComposedRenderer();
    ~NEComposedRenderer(){
        NESafeDelete(_dotLight0);
    }
    
    virtual void prepareDrawMeshes(const std::vector<NEMesh> &meshes);
    
    virtual float colorBlendResult(float color, NEVector3 &position, NEVector3 &normal, void *extraInfo);
public:
    
};

#endif /* NEComposedRenderer_hpp */
