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
    
    NEDotLightRenderer *renderer;
public:
    NEVector3 &position(){return _position;}
    void setPosition(NEVector3 &position){_position = position;}
};

#endif /* NEDotLight_hpp */
