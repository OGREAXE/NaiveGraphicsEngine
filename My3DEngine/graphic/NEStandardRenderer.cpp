//
//  NEStandardRenderer.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEStandardRenderer.hpp"
#include "NECommon.h"
#include <math.h>

NEVector3 vectorFromVertice(const NEVertice & vert){
    NEVector3 v = {vert.x, vert.y, vert.z};
    return v;
}

NEVector3 vectorFromVertice(const NEVertice & vert, float range, float width){
    NEVector3 v = {vert.x * width / range, vert.y * width / range, vert.z * width / range};
    return v;
}

NEVector3 convertNormal(const NEVector3 & normal, const NEMesh &mesh){
    NEVector3 n = normal;
    
    if (mesh.roatation.x != 0) {
        n = rotationByAngle(n, NEVector3Make(1, 0, 0), mesh.roatation.x);
    }
    if (mesh.roatation.y != 0) {
        n = rotationByAngle(n, NEVector3Make(0, 1, 0), mesh.roatation.y);
    }
    if (mesh.roatation.z != 0) {
        n = rotationByAngle(n, NEVector3Make(0, 0, 1), mesh.roatation.z);
    }
    
    return n;
}

NEVector3 vectorFromVertice(const NEVertice & vert, const NEMesh &mesh){
    NEVector3 v = vectorFromVertice(vert, mesh.range, mesh.width);
    
    if (mesh.roatation.x != 0) {
        v = rotationByAngle(v, NEVector3Make(1, 0, 0), mesh.roatation.x);
    }
    if (mesh.roatation.y != 0) {
        v = rotationByAngle(v, NEVector3Make(0, 1, 0), mesh.roatation.y);
    }
    if (mesh.roatation.z != 0) {
        v = rotationByAngle(v, NEVector3Make(0, 0, 1), mesh.roatation.z);
    }
    
    v = translationByVector(v, mesh.position);
    
    return v;
}

NEVector3 NEStandardRenderer::convertToCameraSpace(NEVector3 originalPoint){
    return getPositionInCameraCoordinateSystem(originalPoint, camera.position, camera.lookAtDirection, camera.yAxis);
}

NEVector3 NEStandardRenderer::convertToEyeSpace(NEVector3 originalPoint){
    
    NEVector3 point =  convertToCameraSpace(originalPoint);
    
    NEVector3 pointTran;
    
    if (shouldTrimPoint(point, camera.frustum)) {
        
    }
    
    pointTran = perspetiveProjectPoint(point, camera.frustum);
    
    return pointTran;
}

NEVector3 NEStandardRenderer::convertVectorToEyeSpace(NEVector3 &aVector, NEVector3 &originInEyeSpace){
    NEVector3 vectorT = convertToEyeSpace(aVector);
    return NEVector3Subtract(vectorT, originInEyeSpace);
}

NEVector3 NEStandardRenderer::convertVectorToCameraSpace(NEVector3 &aVector, NEVector3 &originInCameraSpace){
    NEVector3 vectorT = convertToCameraSpace(aVector);
    return NEVector3Subtract(vectorT, originInCameraSpace);
}

//float maxIntensity = 0;
//float minIntensity = 1;

inline NEVector3 mixNormal(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2){
    return NEVector3Make((x0 + x1 + x2)/3., (y0 + y1 + y2)/3., (z0 + z1 + z2)/3.);
}

inline NEVector3 mixNormal(NEVector3 normal0, NEVector3 normal1, NEVector3 normal2){
    return NEVector3Make((normal0.x + normal1.x + normal2.x)/3., (normal0.y + normal1.y + normal2.y)/3., (normal0.z + normal1.z + normal2.z)/3.);
}

inline bool isPointInsideTriangle(NEVector2 &p, NEVector2 &p0, NEVector2 &p1, NEVector2 &p2){
    
#define testPoint(point, start, end) ((point.x - start.x) * (end.y - start.y) - (end.x - start.x) * (point.y - start.y));
    
    int t0 = testPoint(p, p0, p1);
    int t1 = testPoint(p, p1, p2);
    int t2 = testPoint(p, p2, p0);
    
    if ((t0 >= 0 && t1 >= 0 && t2 >= 0) ||
    (t0 <= 0 && t1 <= 0 && t2 <= 0)){
        return true;
    }
    
    return false;
}

static inline float revertScreenHorizatalPos(int screenX, float reverseFactor){
    return (screenX * reverseFactor - 1);
}

static inline float revertScreenVerticalPos(int screenY, float reverseFactor){
    return (1 - screenY * reverseFactor);
}

//void testInvertProject(const NEMesh &mesh){
//    const NEVertice & _v0 = mesh.vertices[0];
//    NEVector3 v0 = vectorFromVertice(_v0, mesh.range, 10);
//
//    NEVector3 v0t = [self convertToEyeSpace:v0];
//
//    NEVector3 v0c = [self convertToCameraSpace:v0];
//    NEVector3 v0c_r = invertPerspetiveProject(v0t, _camera.frustum);
//
//    NEVector3 subtractRes = NEVector3Subtract(v0c, v0c_r);
//    NSLog(@"testInvertProject: %f, %f, %f", subtractRes.x, subtractRes.y, subtractRes.z);
//}

NEVector2 NEStandardRenderer::pointInVewForVector3(NEVector3 vector){
    float width = _screenWidth * COORD_AMPLIFY_FACTOR;
    float height = _screenHeight * COORD_AMPLIFY_FACTOR;
    
    float screen_x = (1 +  vector.x)* width/2;
    float screen_y = ( 1 -  vector.y)* height/2;
    
    return NEVector2Make(screen_x, screen_y);
}


void NEStandardRenderer::drawMeshes(const std::vector<NEMesh> &meshes){
    _depthBuffer.resetSize();
    
    prepareDrawMeshes(meshes);
    
    for (int i = 0; i < meshes.size(); i++) {
        const NEMesh & mesh = meshes[i];
        drawMesh(mesh);
    }
    
    finishDrawMeshes(meshes);
}

void NEStandardRenderer::drawMesh(const NEMesh &mesh){
    NELog("mesh draw begin >>>>>>>>>>>>>>");
//    CGContextRef context = UIGraphicsGetCurrentContext();
//    CGFloat fillWidth = 1./COORD_AMPLIFY_FACTOR;
    
    float screenWidth = _screenWidth * COORD_AMPLIFY_FACTOR;
    float screenHeight = _screenHeight * COORD_AMPLIFY_FACTOR;
    
    NEVector3 originPosC = convertToCameraSpace(NEVector3Make(0, 0, 0));
    
    //do invert projection test here
//    [self testInvertProject:mesh];
//    return;
    //end test
    
    for (const NEFace & aface : mesh.faces) {
        long long color = aface.color;
//        CGContextSetFillColorWithColor(context, HEXRGBCOLOR(aface.color).CGColor);
        
        const NEVertice & _v0 = mesh.vertices[aface.aIndex];
        const NEVertice & _v1 = mesh.vertices[aface.bIndex];
        const NEVertice & _v2 = mesh.vertices[aface.cIndex];
        
        NEVector3 _normal = mixNormal(_v0.normal, _v1.normal, _v2.normal);
        _normal = convertNormal(_normal, mesh);
        
        NEVector3 v0 = vectorFromVertice(_v0, mesh);
        NEVector3 v1 = vectorFromVertice(_v1, mesh);
        NEVector3 v2 = vectorFromVertice(_v2, mesh);
        
        NEVector3 v0c = convertToCameraSpace(v0);
        NEVector3 v1c = convertToCameraSpace(v1);
        NEVector3 v2c = convertToCameraSpace(v2);
        
        NEVector3 v0t = convertToEyeSpace(v0);
        NEVector3 v1t = convertToEyeSpace(v1);
        NEVector3 v2t = convertToEyeSpace(v2);
        
        NEVector3 noraml0 = _v0.normal;
        NEVector3 noraml1 = _v1.normal;
        NEVector3 noraml2 = _v2.normal;
        
        NEVector3 normal0c = convertVectorToCameraSpace(noraml0, originPosC);
        NEVector3 normal1c = convertVectorToCameraSpace(noraml1, originPosC);
        NEVector3 normal2c = convertVectorToCameraSpace(noraml2, originPosC);
        
//        float pointInVew0 = [self pointInVewForVector3:v0t];
//        float pointInVew1 = [self pointInVewForVector3:v1t];
//        float pointInVew2 = [self pointInVewForVector3:v2t];
        
        NEVector2 v0InView = pointInVewForVector3(v0t);
        NEVector2 v1InView = pointInVewForVector3(v1t);
        NEVector2 v2InView = pointInVewForVector3(v2t);
        
        float minx = MIN(MIN(v0InView.x, v1InView.x), v2InView.x);
        float miny = MIN(MIN(v0InView.y, v1InView.y), v2InView.y);
        
        if (minx < 0) {minx = 0;}
        if (miny < 0) {miny = 0;}
        
        float maxx = MAX(MAX(v0InView.x, v1InView.x), v2InView.x);
        float maxy = MAX(MAX(v0InView.y, v1InView.y), v2InView.y);
        
        if (maxx > screenWidth) {maxx = screenWidth;}
        if (maxy > screenHeight) {maxy = screenHeight;}
        
        NEBoundingBox boundingBox;
        boundingBox.startX = (int)minx;
        boundingBox.startY = (int)miny;
        
        boundingBox.endX = (int)maxx;
        boundingBox.endY = (int)maxy;
        
        //normalReal is the normal calculated from 3 vertices
        NEVector3 normalRealt = getPlaneNormal(v0t, v1t, v2t);
        
        //preDefined Normal affects light blend effect
//        NEVector3 preDefinedNormalt = [self convertVectorToEyeSpace:_normal originInEyeSpace:originPosT];
        
        NEVector3 preDefinedNormalC = convertVectorToCameraSpace(_normal, originPosC);
        
        float reverseHorizontalFactor = (1 / (screenWidth/2));
        float reverseVerticalFactor = (1 / (screenHeight/2));
        
        for (int y = boundingBox.startY; y <= boundingBox.endY; y ++) {
            for (int x = boundingBox.startX; x <= boundingBox.endX; x ++) {
                NEVector2 p = NEVector2Make(x, y);
                //performance bottleneck
                if (!isPointInsideTriangle(p, v0InView, v1InView, v2InView)) {
                    continue;
                }
                    
                float revertX = revertScreenHorizatalPos(x, reverseHorizontalFactor);
                float revertY = revertScreenVerticalPos(y, reverseVerticalFactor);
                
                //the point in eye space inside the triangle
                NEVector3 point = getPointInPlane(revertX, revertY, normalRealt, v0t);
                
                NEVector3 pointc = invertPerspetiveProject(point, camera.frustum);
                
                //refering the 3 vertice
                
                long tColor = colorBlendResult(color, pointc, preDefinedNormalC, nullptr);
                
                if (x > _depthBuffer.getWidth() || x < 0 || y > _depthBuffer.getHeight() || y < 0) {
                    continue;
                }
                
                DepthInfo info = _depthBuffer.getInfo(x, y);
                float oldZ = info.z;
                
#define COMPOSE_RENDER_BUF_VAL(x, y, color) ((x | (y << 16)) | (color << 32))
                if (point.z < oldZ) {
                    long oldIndex = info.additionalInfo;
                    if (_renderBuffer) {
                        if (oldIndex > 0) {
                            oldIndex --;
                            _renderBuffer[oldIndex] = COMPOSE_RENDER_BUF_VAL(x, y, tColor)
                            ;
                        } else {
                            _renderBuffer[_renderBufferSize] = COMPOSE_RENDER_BUF_VAL(x, y, tColor);
                            _renderBufferSize ++;
                            
                            info.additionalInfo = _renderBufferSize;
                        }
                    }
                    
                    info.z = point.z;
                    info.color = tColor;
                    
                    _depthBuffer.setInfo(info, x, y);
                } else {
//                    int i = 0;
                }
            }
        }
    }
}

inline float getLightToPointAngle(NEVector3 & point, NEVector3 & lightPosT, NEVector3 & preDefinedNormal){
    NEVector3 lightOnPointVec = NEVector3Make(point.x - lightPosT.x, point.y - lightPosT.y, point.z - lightPosT.z);
    float lightAngle = getAngleBetweenVectors(lightOnPointVec, preDefinedNormal);

    return lightAngle;
}
