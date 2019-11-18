//
//  NEMath.c
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/10/28.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include <stdio.h>
#import <math.h>
#include "NEMath.h"

NEMatrix3 makeRotationMatrix(NEVector3 rotationAxis, float angle){
   return GLKMatrix3MakeRotation(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    float Ux = rotationAxis.x;
    float Uy = rotationAxis.y;
    float Uz = rotationAxis.z;
    
    NEMatrix3 rotm =
    GLKMatrix3Make(/*row0*/
                   cos_a +  Ux * Ux * (1 - cos_a),
                   Ux * Uy * (1 - cos_a) - Uz * sin_a,
                   Ux * Uz * (1 - cos_a) + Uy * sin_a,
                   /*row1*/
                   Uy * Ux * (1 - cos_a) + Uz * sin_a,
                   cos_a +  Uy * Uy * (1 - cos_a),
                   Uy * Uz * (1 - cos_a) - Ux * sin_a,
                   /*row2*/
                   Uz * Ux * (1 - cos_a) - Uy * sin_a,
                   Uz * Uy * (1 - cos_a) + Ux * sin_a,
                   cos_a +  Uz * Uz * (1 - cos_a));
    
    return rotm;
}

///a*b
float vectorMultiply(GLKVector3 vec0,GLKVector3 vec1){
    return vec0.x * vec1.x + vec0.y * vec1.y + vec0.z * vec1.z;
}

///||a||
float vectorMagnitude(GLKVector3 vec){
    return sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
}

float getAngleBetweenVectors(GLKVector3 vec0,GLKVector3 vec1){
    float result = vectorMultiply(vec0, vec1) /(vectorMagnitude(vec0) * vectorMagnitude(vec1));
    result = acosf(result);
    return  result;
}

NEVector3 rotationByMatrix(NEVector3 aPoint, NEMatrix3 rotationMatrix){
    return GLKMatrix3MultiplyVector3(rotationMatrix, aPoint);
}

NEVector3 crossVectors(NEVector3 vec0, NEVector3 vec1){
    NEVector3 cv = {
        vec0.y * vec1.z - vec0.z * vec1.y,
        vec0.z * vec1.x - vec0.x * vec1.z,
        vec0.x * vec1.y - vec0.y * vec1.x
    };
    return cv;
}

NEVector3 reverseVector(NEVector3 vec){
    return GLKVector3Make(- vec.x, - vec.y, - vec.z);
}

NEVector3 translationByVector(NEVector3 aPoint, NEVector3 translationVector){
    return GLKVector3Make(aPoint.x + translationVector.x, aPoint.y + translationVector.y, aPoint.z + translationVector.z);
}

NEVector3 convertPositionFromOriginalCoordSystem(NEVector3 targetOldPosition, NEVector3 coordOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis){
//    return targetOldPosition;
    // 0 + b* 0 + c = 0
    //  zv.x + b * zv.y + c * zv.z = 0;
    // => b = - zv.y / zv.x, c = 0;
    // the axi z-axis will rotate around is (1, b, c)
    // the angle between the 2 z-axis is
    // cos(angle) = ab/|a||b|
    
    //firstly we translate the target origin to world
    NEVector3 newPosition = translationByVector(targetOldPosition, reverseVector(coordOrigin));
    
    //now rotate z axis
    NEVector3 rotAxis = crossVectors(coordZAxis, GLKVector3Make(0,0,1));
    
    float rotZAngle = getAngleBetweenVectors(GLKVector3Make(0, 0, 1), coordZAxis);
    
    NEMatrix3 rotZMat = makeRotationMatrix(rotAxis, rotZAngle);
    
    newPosition = rotationByMatrix(newPosition, rotZMat);
    
    //now that we have rotated z axis, time to rotate y axis
    NEVector3 newCoordYAxis = rotationByMatrix(coordYAxis, rotZMat);
    //angle between coordY axis and world y axis
    float rotYAngle = getAngleBetweenVectors(GLKVector3Make(0, 1, 0), newCoordYAxis);
    NEMatrix3 rotYMat = makeRotationMatrix(GLKVector3Make(0, 0, 1), rotYAngle);
    newPosition = rotationByMatrix(newPosition, rotYMat);
    
    return newPosition;
}

NEVector3 getPositionInCameraCoordinateSystem(NEVector3 worldPosition, NEVector3 cameraPositionInWorld, NEVector3 cameraLookAt, NEVector3 cameraYAxisInWorld){
    NEVector3 pos = convertPositionFromOriginalCoordSystem(worldPosition, cameraPositionInWorld, cameraLookAt, cameraYAxisInWorld);
    
    //rotate it by 180 degrees around y axis to turn into eye space (in accordance with opengl frustum)
    NEVector3 ret = { - pos.x, pos.y, - pos.z};
    return ret;
}

NEVector3 perspetiveProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum){
    NEMatrix4 rotm =
    GLKMatrix4Make(/*row0*/
                   frustum.near/frustum.r,
                   0,
                   0,
                   0,
                   /*row1*/
                   0,
                   frustum.near/frustum.t,
                   0,
                   0,
                   /*row2*/
                   0,
                   0,
                  - (frustum.far + frustum.near) / (frustum.far - frustum.near),
                  - 2 * frustum.far * frustum.near / (frustum.far - frustum.near),
                   /*row3*/
                   0,
                   0,
                   - 1,
                   0);
    
    NEVector4 point4 = GLKVector4MakeWithVector3(pointInCameraSpace, 1);
    
    NEVector4 res = GLKMatrix4MultiplyVector4(rotm, point4);
    
    NEVector3 res3 = GLKVector3Make(res.x / res.w, res.y / res.w, res.z / res.w);
    
    return res3;
}

NEVector3 getVerticalVec(NEVector3 vec, float *px, float *py, float *pz){
    if (!px) {
        if (vec.x != 0) {
            float x = - (vec.y * *py + vec.z * *pz)/vec.x;
            return GLKVector3Make(x, *py, *pz);
        } else {
            return GLKVector3Make(1, 0, 0);
        }
    } else if (!py) {
        if (vec.y != 0) {
            float y = - (vec.x * *px + vec.z * *pz)/vec.y;
            return GLKVector3Make(*px, y, *pz);
        } else {
            return GLKVector3Make(0, 1, 0);
        }
    } else if (!pz) {
       if (vec.z != 0) {
           float z = - (vec.x * *px + vec.y * *py)/vec.z;
           return GLKVector3Make(*px, *py, z);
       } else {
           return GLKVector3Make(0, 0, 1);
       }
    } else {
        return GLKVector3Make(0, 0, 0);
    }
}

NE_RESULT getPointsArrayInLine(NEVector3 start, NEVector3 end, NEVector3 * pointsBuf, int maxBufSize, int * bufSize){
    *bufSize = 0;
    
    CGFloat step = 0.002;
    
    if (start.x == end.x && start.y != end.y) {
        float k_yz = (end.z - start.z)/( end.y - start.y);
        for (float s =  MIN(start.y, end.y) ; s < MAX(start.y, end.y) ; s += step) {
            float z = k_yz * ( s - start.y) + start.z;
            pointsBuf[*bufSize] = GLKVector3Make(start.x, s, z);
            *bufSize = *bufSize + 1;
        }
        
        float max_y = MAX(start.y, end.y);
        pointsBuf[*bufSize] = GLKVector3Make(start.x, max_y, max_y == start.y?start.z:end.z);
        *bufSize = *bufSize + 1;
        
        return NE_RESULT_OK;
    }
    
    if (start.y == end.y && start.x != end.x) {
        float k_xz = (end.z - start.z)/( end.x - start.x);
        for (float s =  MIN(start.x, end.x) ; s < MAX(start.x, end.x) ; s += step) {
            float z = k_xz * ( s - start.x) + start.z;
            pointsBuf[*bufSize] = GLKVector3Make(s, start.y, z);
            *bufSize = *bufSize + 1;
        }
        
        float max_x = MAX(start.x, end.x);
        pointsBuf[*bufSize] = GLKVector3Make(max_x, start.y, max_x == start.x?start.z:end.z);
        *bufSize = *bufSize + 1;
        
        return NE_RESULT_OK;
    }
    
    if (start.x != end.x && start.y != end.y) {
        float k = (end.y - start.y)/( end.x - start.x);
        float k_xz = (end.z - start.z)/( end.x - start.x);
        
        for (float s =  MIN(start.x, end.x) ; s < MAX(start.x, end.x) ; s += step) {
            float y = k * ( s - start.x) + start.y;
            float z = k_xz * ( s - start.x) + start.z;
            pointsBuf[*bufSize] = GLKVector3Make(s, y, z);
            *bufSize = *bufSize + 1;
        }
        
        if (start. x < end.x) {
            pointsBuf[*bufSize] = GLKVector3Make(end.x, end.y, end.z);
            *bufSize = *bufSize + 1;
        } else {
            pointsBuf[*bufSize] = GLKVector3Make(start.x, start.y, start.z);
            *bufSize = *bufSize + 1;
        }
        
        return NE_RESULT_OK;
    }
    
    return NE_RESULT_FAIL;
    
}
