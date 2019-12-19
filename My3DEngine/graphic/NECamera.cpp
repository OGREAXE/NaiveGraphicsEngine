//
//  NECamera.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NECamera.hpp"

#define NEAR_FACTOR 1

NECamera::NECamera(){
    NEFrustum fru;
    fru.r = 0.2; //x
    fru.t = 0.2; //y
    
    fru.near = 1;
    fru.far = 100;
    
    this->frustum = fru;
}

void NECamera::setWindow(float width, float height){
    NEFrustum fru;
    
    if (_isOthorgraphics) {
        fru.r = 15; //x
        fru.t = fru.r * height/width; //y
    } else {
        fru.r = 0.2; //x
        fru.t = fru.r * height/width; //y
    }
    
    fru.near = 1;
    fru.far = 100;
    
    this->frustum = fru;
    
    initProjectMatrix();
}

void NECamera::initProjectMatrix(){
    _perspectiveProjectionMatrix =
    NEMatrix4Make(/*col0*/
                   frustum.near/frustum.r,
                   0,
                   0,
                   0,
                   /*col01*/
                   0,
                   frustum.near/frustum.t,
                   0,
                   0,
                   /*col02*/
                   0,
                   0,
                  - (frustum.far + frustum.near) / (frustum.far - frustum.near),
                  - 1,
                   /*col03*/
                   0,
                   0,
                   - 2 * frustum.far * frustum.near / (frustum.far - frustum.near),
                   0);
    
    _othorgraphicsProjectionMatrix =
    NEMatrix4Make(/*col0*/
                   1 / frustum.r,
                   0,
                   0,
                   0,
                   /*col01*/
                   0,
                   1 / frustum.t,
                   0,
                   0,
                   /*col02*/
                   0,
                   0,
                  - 2 / (frustum.far - frustum.near),
                   0,
                   /*col03*/
                   0,
                   0,
                   - (frustum.far + frustum.near) / (frustum.far - frustum.near),
                   1);
    
    _perspetiveInvertProjection.m00 = frustum.near / frustum.r;
    _perspetiveInvertProjection.m11 = frustum.near / frustum.t;
    _perspetiveInvertProjection.m22 = - (frustum.far + frustum.near) / (frustum.far - frustum.near);
    _perspetiveInvertProjection.m23 = - 2 * frustum.far * frustum.near / (frustum.far - frustum.near);
    
    _othorInvertProjection.m00 = 1 / frustum.r;
    _othorInvertProjection.m11 = 1 / frustum.t;
    _othorInvertProjection.m22 = - 2 / (frustum.far - frustum.near);
    _othorInvertProjection.m23 = - (frustum.far + frustum.near) / (frustum.far - frustum.near);
}

void NECamera::lookAtPoint(NEVector3 pointToLookAt){
    NEVector3 direction = NEVector3Make(pointToLookAt.x - this->position.x, pointToLookAt.y - this->position.y, pointToLookAt.z - this->position.z);
    
    this->lookAtDirection = direction;
    
    NEVector3 lookAtRotated_90= { - lookAtDirection.y , lookAtDirection.x, 0};
    NEVector3 cam_yAxis = vectorCrossProduct(lookAtDirection, lookAtRotated_90);
    this->yAxis = cam_yAxis;
    this->xAxis = lookAtRotated_90;
    
    this->normalize();
}

void NECamera::normalize(){
    this->lookAtDirection = getNormalizedVector(this->lookAtDirection);
    this->xAxis = getNormalizedVector(this->xAxis);
    this->yAxis = getNormalizedVector(this->yAxis);
}

void NECamera::rotateByNearHorizontallyByDegree(float degree){
    this->normalize();
    
    float nearDistance = this->frustum.near * NEAR_FACTOR;
    NEVector3 pointInRotatedAxis = NEVector3Make(this->position.x + nearDistance * this->lookAtDirection.x, this->position.y + nearDistance * this->lookAtDirection.y, this->position.z + nearDistance * this->lookAtDirection.z);
    
    this->xAxis = rotationByAngle(this->xAxis, this->yAxis, degree);
    this->lookAtDirection = rotationByAngle(this->lookAtDirection, this->yAxis, degree);
    
    this->position = rotationByAngleAroundLine2(this->position, this->yAxis, pointInRotatedAxis, degree);
}

void NECamera::rotateByNearVerticallyByDegree(float degree){
    this->normalize();
    
    float nearDistance = this->frustum.near * NEAR_FACTOR;
    NEVector3 pointInRotatedAxis = NEVector3Make(this->position.x + nearDistance * this->lookAtDirection.x, this->position.y + nearDistance * this->lookAtDirection.y, this->position.z + nearDistance * this->lookAtDirection.z);
    
    this->yAxis = rotationByAngle(this->yAxis, this->xAxis, degree);
    this->lookAtDirection = rotationByAngle(this->lookAtDirection, this->xAxis, degree);
    
    this->position = rotationByAngleAroundLine2(this->position, this->xAxis, pointInRotatedAxis, degree);
}


NEVector3 NECamera::projectPoint(NEVector3 &pointInCameraSpace){
    if (_isOthorgraphics) {
        NEVector4 point4 = NEVector4MakeWithVector3(pointInCameraSpace, 1);
        NEVector4 res = NEMatrix4MultiplyVector4(_othorgraphicsProjectionMatrix, point4);
        return NEVector3Make(res.x / res.w, res.y / res.w, res.z / res.w);
    } else {
        NEVector4 point4 = NEVector4MakeWithVector3(pointInCameraSpace, 1);
        NEVector4 res = NEMatrix4MultiplyVector4(_perspectiveProjectionMatrix, point4);
        return NEVector3Make(res.x / res.w, res.y / res.w, res.z / res.w);
    }
}

NEVector3 NECamera::invertProject(NEVector3 &pointInEyeSpace){
    if (_isOthorgraphics) {
        float ze = (pointInEyeSpace.z - _othorInvertProjection.m23) / _othorInvertProjection.m22;
        float xe = pointInEyeSpace.x / _othorInvertProjection.m00;
        float ye = pointInEyeSpace.y / _othorInvertProjection.m11;
        
        return NEVector3Make(xe, ye, ze);
    } else {
        float ze = - _perspetiveInvertProjection.m23 / (pointInEyeSpace.z + _perspetiveInvertProjection.m22);
        float xe = pointInEyeSpace.x * (-ze) / _perspetiveInvertProjection.m00;
        float ye = pointInEyeSpace.y * (-ze) / _perspetiveInvertProjection.m11;
        
        return NEVector3Make(xe, ye, ze);
    }
}
