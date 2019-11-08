//
//  NEMath.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEMath_h
#define NEMath_h

#import <GLKit/GLKit.h>

#define EXPORT_PREFIX extern "C"

///vector definitions
typedef GLKVector2 NEVector2;
typedef GLKVector3 NEVector3;
typedef GLKVector4 NEVector4;

///matrix definitions
typedef GLKMatrix3 NEMatrix3;
typedef GLKMatrix4 NEMatrix4;

typedef struct tagNEFrustum{
    float near;
    float far;
    float l; //1/2 height of the near window
} NEFrustum;

NEMatrix3 makeRotationMatrix(NEVector3 rotationAxis, float angle);

///a*b
float vectorMultiply(GLKVector3 vec0,GLKVector3 vec1);

///||a||
float vectorMagnitude(GLKVector3 vec);

float getAngleBetweenVectors(GLKVector3 vec0,GLKVector3 vec1);

NEVector3 rotationByMatrix(NEVector3 aPoint, NEMatrix3 rotationMatrix);

NEVector3 reverseVector(NEVector3 vec);

NEVector3 translationByVector(NEVector3 aPoint, NEVector3 translationVector);

NEVector3 convertPositionFromOriginalCoordSystem(NEVector3 targetOldPosition, NEVector3 coordOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis);

NEVector3 getPositionInCameraCoordinateSystem(NEVector3 worldPosition, NEVector3 cameraPositionInWorld, NEVector3 cameraLookAt, NEVector3 cameraYAxisInWorld);

NEVector2 perspetiveProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum);

#endif /* NEMath_h */
