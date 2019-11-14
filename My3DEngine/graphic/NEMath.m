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
    return convertPositionFromOriginalCoordSystem(worldPosition, cameraPositionInWorld, cameraLookAt, cameraYAxisInWorld);
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
                   1,
                   0);
    
    NEVector4 point4 = GLKVector4MakeWithVector3(pointInCameraSpace, 1);
    
    NEVector4 res = GLKMatrix4MultiplyVector4(rotm, point4);
    
    NEVector3 res3 = GLKVector3Make(res.x / res.w, res.y / res.w, res.z / res.w);
    
    return res3;
}