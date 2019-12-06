//
//  NEComposedRenderer.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEComposedRenderer.hpp"
#include <math.h>

#define NE_TRUE_LIGHT

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
    
    r *= intensity * intensity;
    g *= intensity * intensity;
    b *= intensity * intensity;
    
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
    
}

void NEComposedRenderer::prepareDrawMeshes(const std::vector<NEMesh> &meshes){
    camera.updateWorldAxis();
    
    _dotLight0->prepareRander();
    
    NEVector3 &lightPos = _dotLight0->position();
    
    _dotLightPositionInCameraSpace  = convertToCameraSpace(lightPos);
    
    _device->clearDevice(NEVector4Make(0, 0, _screenWidth, _screenHeight));
    
    _renderBufferSize = 0;
    
    _dotLight0->renderIfNeed(meshes);
}

void NEComposedRenderer::finishDrawMeshes(const std::vector<NEMesh> &meshes){
    _device->presentRenderBuffer(_renderBuffer, _renderBufferSize, 0, nullptr);
}

float NEComposedRenderer::colorBlendResult(float color, NEVector3 &position, NEVector3 &normal, void *extraInfo){
#ifdef NE_TRUE_LIGHT
    
    NEVector3 worldPos = camera.getWorldPosition(position);
    
    if (!_dotLight0->canTouchPosition(worldPos)) {
        float lightAngle = M_PI_2;
        long tColor = getColorWithIntensity(color,  (1. - cosf(lightAngle))/2.);
        return tColor;
    }
#endif
    
    return generateBlendResultWithColor(color, position, _dotLightPositionInCameraSpace, normal);
}
