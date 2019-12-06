//
//  NEComposedRenderer.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEComposedRenderer.hpp"
#include <math.h>

inline long getColorWithIntensity(long color, float intensity){
//    if (intensity > maxIntensity) {
//        maxIntensity = intensity;
//    }
//
//    if (intensity < minIntensity) {
//        minIntensity = intensity;
//    }
    
    long r = (color >>16) & 0xff;
    long g = (color >>8) & 0xff;
    long b = color & 0xff;
    
    r *= intensity;
    g *= intensity;
    b *= intensity;
    
//    if (intensity < 0.01){
//        int k = 0;
//    }
    
    return r <<16 | g <<8 | b;
}

inline float getLightToPointAngle(NEVector3 & point, NEVector3 & lightPosT, NEVector3 & preDefinedNormal){
    NEVector3 lightOnPointVec = NEVector3Make(point.x - lightPosT.x, point.y - lightPosT.y, point.z - lightPosT.z);
    float lightAngle = getAngleBetweenVectors(lightOnPointVec, preDefinedNormal);

    return lightAngle;
}

inline long generateBlendResultWithColor(long color, NEVector3 & point, NEVector3 & lightPos, NEVector3 & normal){
    float lightAngle = getLightToPointAngle(point, lightPos, normal);
    
    if(lightAngle < M_PI_2){
        // smaller than pi/2, light is from behind
        lightAngle = M_PI_2;
    }
    
    long tColor = getColorWithIntensity(color,  (1. - cosf(lightAngle))/2.);
    
    return tColor;
}

NEComposedRenderer::NEComposedRenderer(){
    this->_dotLight0 = new NEDotLight();
}

void NEComposedRenderer::prepareDrawMeshes(const std::vector<NEMesh> &meshes){
    NEVector3 &lightPos = _dotLight0->position();
    _dotLightPositionInCameraSpace  = convertToCameraSpace(lightPos);
}

float NEComposedRenderer::colorBlendResult(float color, NEVector3 &position, NEVector3 &normal, void *extraInfo){
    
    return generateBlendResultWithColor(color, position, _dotLightPositionInCameraSpace, normal);
}
