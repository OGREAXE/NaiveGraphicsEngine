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
