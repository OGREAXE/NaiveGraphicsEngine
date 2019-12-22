//
//  NEComposedRenderer.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEComposedRenderer.hpp"
#include <math.h>
#include "NEMesh.h"

typedef struct tagPosition2dWithPayload {
    NEVector2 vec;
    float payload[kPayloadMaxCount];
} Position2DWithPayload;

typedef struct tagNLine {
    Position2DWithPayload *start;
    Position2DWithPayload *end;
} NLine;

inline float getJointXByY(float y, NEVector2 &lstart, NEVector2 &lend){
    if(lstart.x == lend.x){
        return lstart.x;
    }
 
    float x = (y - lstart.y ) * (lend.x - lstart.x) / (lend.y - lstart.y) + lstart.x;
    
    return x;
}

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
    
    r *= intensity ;
    g *= intensity ;
    b *= intensity ;
    
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

void NEComposedRenderer::prepareDrawFace(const NEFace &face, ShaderParam &param){
    float lightAngle0 = getLightToPointAngle(param.vert0c, _dotLightPositionInCameraSpace, param.normal0c);
    float lightAngle1 = getLightToPointAngle(param.vert1c, _dotLightPositionInCameraSpace, param.normal1c);
    float lightAngle2 = getLightToPointAngle(param.vert2c, _dotLightPositionInCameraSpace, param.normal2c);
    
    param.intensity0 = getIntensityFromLightAngle(lightAngle0);
    param.intensity1 = getIntensityFromLightAngle(lightAngle1);
    param.intensity2 = getIntensityFromLightAngle(lightAngle2);
    
    //prepare uv params
    param.material.uv0_z = NEVector2Make(param.material.uv0.x/param.vert0c.z, param.material.uv0.y/param.vert0c.z);
    
    param.material.uv1_z = NEVector2Make(param.material.uv1.x/param.vert1c.z, param.material.uv1.y/param.vert1c.z);
    
    param.material.uv2_z = NEVector2Make(param.material.uv2.x/param.vert2c.z, param.material.uv2.y/param.vert2c.z);
}

void NEComposedRenderer::finishDrawMeshes(const std::vector<NEMesh> &meshes){
    _device->presentRenderBuffer(_renderBuffer, _renderBufferSize, 0, nullptr);
}

#ifdef NE_USE_AVG_INTENSITY

//when texture mapping is enable, mesh color is ignored
//float NEComposedRenderer::colorBlendResult(float color, NEVector3 &position,  DrawParam &param, void *extraInfo){
//
//    if (param.textureParam.hasTexture) {
//        //handle texture
//        float texture_u = position.z * getInterpolatedValueForTriangle3(param.position_t, param.vert0t, param.vert1t, param.vert2t, param.textureParam.uv0_z.x, param.textureParam.uv1_z.x, param.textureParam.uv2_z.x);
//
//        float texture_v = position.z * getInterpolatedValueForTriangle3(param.position_t, param.vert0t, param.vert1t, param.vert2t, param.textureParam.uv0_z.y, param.textureParam.uv1_z.y, param.textureParam.uv2_z.y);
//
//        color = _textureProvider->readColorFromTexture(param.textureParam.textureIndex, texture_u, texture_v);
//    }
//
//    ///
//    float fade = 1;
//
//    if (_trueShadow) {
//        NEVector3 worldPos = camera.getWorldPosition(position);
//        if (!_dotLight0->canTouchPosition(worldPos)) {
//            fade = 0.7;
//        }
//    }
//
//    /*
//     float lightAngle = getLightToPointAngle(position, _dotLightPositionInCameraSpace, normal);
//
//    if(lightAngle < M_PI_2){
//        fade = 0.9;
//    }
//     */
//
//    float intensity = getInterpolatedValueForTriangle3(position, param.vert0c, param.vert1c, param.vert2c, param.intensity0, param.intensity1, param.intensity2);
//
//    long tColor = getColorWithIntensity(color, fade * intensity);
//
//    return tColor;
//}

inline NELong calSpecularColor(NELong color, NELong specColor, NELong gloss, ShaderParam &param, NECamera &camera, float distanceFactor, NEVector3 & worldPos, NEVector3 &lightPos){
    float worldNormalx = (param.payload_interpl_length[NE_PAYLOAD_NORMAL_X_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_NORMAL_X_INDEX]);
    float worldNormaly = (param.payload_interpl_length[NE_PAYLOAD_NORMAL_Y_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_NORMAL_Y_INDEX]);
    float worldNormalz = (param.payload_interpl_length[NE_PAYLOAD_NORMAL_Z_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_NORMAL_Z_INDEX]);
    
    NEVector3 worldNormal = NENormalize(NEVector3Make(worldNormalx, worldNormaly, worldNormalz));
    
//                specColor = 0xffffff;
//                float gloss = 3;
    
    NEVector3 viewDir = NENormalize(NEVector3Subtract(camera.position, worldPos));
    NEVector3 lightDir = NENormalize(NEVector3Subtract(lightPos, worldPos));
    NEVector3 halfDir = NENormalize(NEVector3Add(lightDir, viewDir));
    
    float specAngle = saturate(NEVector3DotProduct(halfDir, worldNormal));
    float f = pow(specAngle, gloss);
    
    color = colorAdd(color, colorMul(specColor, f));
    
    return color;
}


float NEComposedRenderer::FragmentShaderFunc(float color, NEVector3 &position,  ShaderParam &param, void *extraInfo){
    
    NEMaterialParam & materialParam = param.material;
    
    NEMaterial & material = _materials[materialParam.materialIndex];
    
    if (material.diffuse.exist) {
        color = material.diffuse.value;
    }
    
    if (materialParam.hasTexture) {
        if (materialParam.textureStack.size() == 0) {
            NETexture tex;
            for (int i : material.textureStack) {
                if(_textureProvider->getTexture(i, tex)){
                    materialParam.textureStack.push_back(tex);
                }
            }
        }
    }
    
    int payloadSize = 6;
    
    if (param.newLine) {
        param.newLine = false;
        
        NEVector3 &positionT = param.position_t;
        
        NEVector2 v0t = {param.vert0t.x, param.vert0t.y};
        NEVector2 v1t = {param.vert1t.x, param.vert1t.y};
        NEVector2 v2t = {param.vert2t.x, param.vert2t.y};
        
        Position2DWithPayload v0 = {v0t, param.material.uv0_z.x, param.material.uv0_z.y, param.intensity0, param.normal0.x, param.normal0.y, param.normal0.z};
        Position2DWithPayload v1 = {v1t, param.material.uv1_z.x, param.material.uv1_z.y, param.intensity1, param.normal1.x, param.normal1.y, param.normal1.z};
        Position2DWithPayload v2 = {v2t, param.material.uv2_z.x, param.material.uv2_z.y, param.intensity2, param.normal2.x, param.normal2.y, param.normal2.z};
        
        NLine line0 = {&v0, &v1};
        NLine line1 = {&v0, &v2};
        NLine line2 = {&v1, &v2};
        
        NLine *lineLeft = NULL, *lineRight = NULL;
        
        if ((line0.start->vec.y - positionT.y) * (line0.end->vec.y - positionT.y) >= 0) {
            lineLeft = &line1;
            lineRight = &line2;
        } else if ((line1.start->vec.y - positionT.y) * (line1.end->vec.y - positionT.y) >= 0) {
            lineLeft = &line0;
            lineRight = &line2;
        } else if ((line2.start->vec.y - positionT.y) * (line2.end->vec.y - positionT.y) >= 0) {
            lineLeft = &line0;
            lineRight = &line1;
        }
        
        Position2DWithPayload jointLeft, jointRight;
        
        float jxl = getJointXByY(positionT.y, (*lineLeft).start->vec, (*lineLeft).end->vec);
        float jxr = getJointXByY(positionT.y, (*lineRight).start->vec, (*lineRight).end->vec);
        
        if (jxl > jxr) {
            NLine *temp = lineLeft;
            lineLeft = lineRight;
            lineRight = temp;
            
            float tmpJ = jxl;
            jxl = jxr;
            jxr = tmpJ;
        }
        
        jointLeft.vec.x = jxl;
        jointLeft.vec.y = positionT.y;
        
        jointRight.vec.x = jxr;
        jointRight.vec.y = positionT.y;
        
        {
            float l_es = NEVector2Distance(lineLeft->start->vec, lineLeft->end->vec);
            float l_js = NEVector2Distance(lineLeft->start->vec, jointLeft.vec);
            float ratioL = l_js / l_es;
            
//            jointLeft.payload0 = (lineLeft->end->payload0 - lineLeft->start->payload0) * ratioL + lineLeft->start->payload0;
//            jointLeft.payload1 = (lineLeft->end->payload1 - lineLeft->start->payload1) * ratioL + lineLeft->start->payload1;
//            jointLeft.payload2 = (lineLeft->end->payload2 - lineLeft->start->payload2 ) * ratioL + lineLeft->start->payload2;
//
            for (int i = 0; i < payloadSize; i++) {
                jointLeft.payload[i] = (lineLeft->end->payload[i] - lineLeft->start->payload[i]) * ratioL + lineLeft->start->payload[i];
            }
        }
        {
            float r_es = NEVector2Distance(lineRight->start->vec, lineRight->end->vec);
            float r_js = NEVector2Distance(lineRight->start->vec, jointRight.vec);
            float ratioR = r_js / r_es;
            
//            jointRight.payload0 = (lineRight->end->payload0 - lineRight->start->payload0) * ratioR + lineRight->start->payload0;
//            jointRight.payload1 = (lineRight->end->payload1 - lineRight->start->payload1) * ratioR + lineRight->start->payload1;
//            jointRight.payload2 = (lineRight->end->payload2 - lineRight->start->payload2) * ratioR + lineRight->start->payload2;
            
            for (int i = 0; i < payloadSize; i++) {
                jointRight.payload[i] = (lineRight->end->payload[i] - lineRight->start->payload[i]) * ratioR + lineRight->start->payload[i];
            }
        }
        
//        materialParam.interpl_uz_start = jointLeft.payload0;
//        materialParam.interpl_vz_start = jointLeft.payload1;
//        param.interpl_intensity_start = jointLeft.payload2;
//
//
//
//        materialParam.interpl_uz_diff = materialParam.interpl_uz_end - materialParam.interpl_uz_start;
//        materialParam.interpl_vz_diff = materialParam.interpl_vz_end - materialParam.interpl_vz_start;
//        param.interpl_intensity_diff = param.interpl_intensity_end - param.interpl_intensity_start;
        
        for (int i = 0; i < payloadSize; i++) {
            param.payload_interpl_start[i] = jointLeft.payload[i];
            param.payload_interpl_length[i] = jointRight.payload[i] - jointLeft.payload[i];
        }
        
        param.interpolate_jointLeft = jointLeft.vec;
        param.interpolate_jointRight = jointRight.vec;
        
        param.interpl_divide_factor = 1/(param.interpolate_jointRight.x - param.interpolate_jointLeft.x);
    }
    
    float distanceFactor = (param.position_t.x - param.interpolate_jointLeft.x) * param.interpl_divide_factor;
    
    NEVector3 worldPos = camera.getWorldPosition(position);
    
    float shadowFade = 1;

    if (_trueShadow) {
        if (!_dotLight0->canTouchPosition(worldPos)) {
            shadowFade = 0.7;
        }
    }
    
    float intensity = param.payload_interpl_length[NE_PAYLOAD_INTENSITY_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_INTENSITY_INDEX];
    
    
    if (material.textureStack.size() > 0) {
        //handle texture
//        float texture_u = position.z * (materialParam.interpl_uz_diff * distanceFactor + materialParam.interpl_uz_start);
//        float texture_v = position.z * (materialParam.interpl_vz_diff * distanceFactor + materialParam.interpl_vz_start);
        
        float texture_u = position.z * (param.payload_interpl_length[NE_PAYLOAD_UZ_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_UZ_INDEX]);
        float texture_v = position.z * (param.payload_interpl_length[NE_PAYLOAD_VZ_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_VZ_INDEX]);

        for (int i = 0; i < material.textureStack.size(); i++) {
            NETexture & texture = materialParam.textureStack[i];
            if (texture.type == NETextureType_DIFFUSE) {
                float diffuseColor = _textureProvider->readColorFromTexture(material.textureStack[i], texture_u, texture_v);
                color = diffuseColor;
                color = getColorWithIntensity(color, shadowFade * intensity);
            }
            else if (texture.type == NETextureType_SPECULAR) {
                
                NELong specColor = _textureProvider->readColorFromTexture(material.textureStack[i], texture_u, texture_v);
                float gloss = material.gloss.value;
                
//                float worldNormalx = (param.payload_interpl_length[NE_PAYLOAD_NORMAL_X_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_NORMAL_X_INDEX]);
//                float worldNormaly = (param.payload_interpl_length[NE_PAYLOAD_NORMAL_Y_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_NORMAL_Y_INDEX]);
//                float worldNormalz = (param.payload_interpl_length[NE_PAYLOAD_NORMAL_Z_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_NORMAL_Z_INDEX]);
//
//                NEVector3 worldNormal = NENormalize(NEVector3Make(worldNormalx, worldNormaly, worldNormalz));
//
////                specColor = 0xffffff;
////                float gloss = 3;
//
//                NEVector3 viewDir = NENormalize(NEVector3Subtract(camera.position, worldPos));
//                NEVector3 lightDir = NENormalize(NEVector3Subtract(_dotLight0->position(), worldPos));
//                NEVector3 halfDir = NENormalize(NEVector3Add(lightDir, viewDir));
//
//                float specAngle = saturate(NEVector3DotProduct(halfDir, worldNormal));
//                float f = pow(specAngle, gloss);
//
//                color = colorAdd(color, colorMul(specColor, f));
                
                color = calSpecularColor(color, specColor, gloss, param, camera, distanceFactor, worldPos, _dotLight0->position());
            }
        }
    } else {
        if (material.diffuse.exist) {
            NELong diffuse = material.diffuse.value;
            color = diffuse;
        }
        
        if (material.specular.exist) {
            NELong specColor = material.specular.value;
            float gloss = material.gloss.value;
            color = calSpecularColor(color, specColor, gloss, param, camera, distanceFactor, worldPos, _dotLight0->position());
        }
        
        color = getColorWithIntensity(color, shadowFade * intensity);
    }
    
    ///
//    float shadowFade = 1;
//
//    if (_trueShadow) {
//        if (!_dotLight0->canTouchPosition(worldPos)) {
//            shadowFade = 0.7;
//        }
//    }
//
//    float intensity = param.payload_interpl_length[NE_PAYLOAD_INTENSITY_INDEX] * distanceFactor + param.payload_interpl_start[NE_PAYLOAD_INTENSITY_INDEX];
//
//    long tColor = getColorWithIntensity(color, shadowFade * intensity);
    
    return color;
}

#else

float NEComposedRenderer::colorBlendResult(float color, NEVector3 &position, DrawParam &param, void *extraInfo){
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
