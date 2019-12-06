//
//  NEDotLight.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEDotLight_hpp
#define NEDotLight_hpp

#include <stdio.h>
#include "NEDotLightRenderer.hpp"
#include "NEMath.h"

class NEDotLight {
    NEVector3 _position;
    
    NEDotLightRenderer *_renderer;
    
    bool _needRedrawMap;
public:
    NEDotLight(){
        _renderer = new NEDotLightRenderer(500, 500);
        _needRedrawMap = true;
    }
    ~NEDotLight(){
        delete _renderer;
    }
    
    NEVector3 &position(){return _position;}
    void setPosition(NEVector3 &position){
        _position = position;
        _renderer->camera.position = position;
    }
    
    void setRenderDestination(NEVector3 destinationPoint){
        _renderer->camera.lookAtPoint(destinationPoint);
    }
    
    void prepareRander(){
        _renderer->camera.updateWorldAxis();
    }
    
    void renderIfNeed(const std::vector<NEMesh> &meshes);
    
    void setNeedUpdate(){
        _needRedrawMap = true;
    }
    
    bool canTouchPosition(NEVector3 &worldPos){
        int res =_renderer->testPosition_world(worldPos);
        
        return res == 1;
    }
};

#endif /* NEDotLight_hpp */
