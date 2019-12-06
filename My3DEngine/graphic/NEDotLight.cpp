//
//  NEDotLight.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEDotLight.hpp"

void NEDotLight::renderIfNeed(const std::vector<NEMesh> &meshes){
    if (!_needRedrawMap) {
        return;
    }
    
    _renderer->clear();
    
    _renderer->drawMeshes(meshes);
    
    _needRedrawMap = false;
}
