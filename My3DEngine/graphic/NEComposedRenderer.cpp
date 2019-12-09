//
//  NEComposedRenderer.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEComposedRenderer.hpp"
#include <math.h>

#define NE_USE_AVG_INTENSITY

//#define NE_TRUE_LIGHT

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

inline float getIntensityFromLightAngle(float angle){
    return (1. - cosf(angle))/2.;
}

inline long generateBlendResultWithColor(long color, NEVector3 & point, NEVector3 & lightPos, NEVector3 & normal, float fade){
    float lightAngle = getLightToPointAngle(point, lightPos, normal);
    
    if(lightAngle < M_PI_2){
        // smaller than pi/2, light is from behind
//        lightAngle = M_PI_2;
        
        fade = 0.9;
    }
    
    float intensity = getIntensityFromLightAngle(lightAngle);
    long tColor = getColorWithIntensity(color, fade * intensity);
    
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

void NEComposedRenderer::prepareDrawFace(const NEFace &face, DrawParam &param){
    float lightAngle0 = getLightToPointAngle(param.vert0c, _dotLightPositionInCameraSpace, param.normal0c);
    float lightAngle1 = getLightToPointAngle(param.vert1c, _dotLightPositionInCameraSpace, param.normal1c);
    float lightAngle2 = getLightToPointAngle(param.vert2c, _dotLightPositionInCameraSpace, param.normal2c);
    
    param.intensity0 = getIntensityFromLightAngle(lightAngle0);
    param.intensity1 = getIntensityFromLightAngle(lightAngle1);
    param.intensity2 = getIntensityFromLightAngle(lightAngle2);
}

void NEComposedRenderer::finishDrawMeshes(const std::vector<NEMesh> &meshes){
    _device->presentRenderBuffer(_renderBuffer, _renderBufferSize, 0, nullptr);
}

#ifdef NE_USE_AVG_INTENSITY

float NEComposedRenderer::colorBlendResult(float color, NEVector3 &position, NEVector3 &normal,  DrawParam &param, void *extraInfo){
        
    float fade = 1;

#ifdef NE_TRUE_LIGHT
    NEVector3 worldPos = camera.getWorldPosition(position);
    if (!_dotLight0->canTouchPosition(worldPos)) {
        fade = 0.9;
    }
#endif
    
    /*
     float lightAngle = getLightToPointAngle(position, _dotLightPositionInCameraSpace, normal);
    
    if(lightAngle < M_PI_2){
        fade = 0.9;
    }
     */

    float intensity = getIntensityForTriangle3(position, param.vert0c, param.vert1c, param.vert2c, param.intensity0, param.intensity1, param.intensity2);
    
    long tColor = getColorWithIntensity(color, fade * intensity);
    
    return tColor;
}

#else

float NEComposedRenderer::colorBlendResult(float color, NEVector3 &position, NEVector3 &normal,  DrawParam &param, void *extraInfo){
#ifdef NE_TRUE_LIGHT
    
    NEVector3 worldPos = camera.getWorldPosition(position);
    
    float fade = 1;
    
    if (!_dotLight0->canTouchPosition(worldPos)) {
        fade = 0.8;
    }
#endif

    return generateBlendResultWithColor(color, position, _dotLightPositionInCameraSpace, normal, fade);
}

#endif
