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

#ifdef __cplusplus
extern "C"{
#endif

#define NE_RESULT int
#define NE_RESULT_FAIL 1
#define NE_RESULT_OK 0

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
    float t;  //1/2 height of the near window
    float r;  //1/2 width of the near window
} NEFrustum;

typedef struct tagNELine{
    NEVector3 vector;
    NEVector3 pointInLine;
} NELine;

NEMatrix3 makeRotationMatrix(NEVector3 rotationAxis, float angle);

///a*b
float vectorMultiply(GLKVector3 vec0,GLKVector3 vec1);

NEVector3 crossVectors(NEVector3 vec0, NEVector3 vec1);

///||a||
float vectorMagnitude(GLKVector3 vec);

float getAngleBetweenVectors(GLKVector3 vec0,GLKVector3 vec1);

NEVector3 rotationByAngle(NEVector3 aPoint, NEVector3 rotationAxis, float angle);

NEVector3 rotationByAngleAroundLine(NEVector3 aPoint, NELine line, float angle);

NEVector3 rotationByAngleAroundLine2(NEVector3 aPoint, NEVector3 lineVec, NEVector3 aPointInLine, float angle);

NEVector3 rotationByMatrix(NEVector3 aPoint, NEMatrix3 rotationMatrix);

NEVector3 reverseVector(NEVector3 vec);

NEVector3 translationByVector(NEVector3 aPoint, NEVector3 translationVector);

NEVector3 convertPositionFromOriginalCoordSystem(NEVector3 targetOldPosition, NEVector3 coordOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis);

NEVector3 getPositionInCameraCoordinateSystem(NEVector3 worldPosition, NEVector3 cameraPositionInWorld, NEVector3 cameraLookAt, NEVector3 cameraYAxisInWorld);

NEVector3 perspetiveProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum);

NEVector3 getVerticalVec(NEVector3 vec, float *x, float *y, float *z);

NE_RESULT getPointsArrayInLine(NEVector3 start, NEVector3 end, NEVector3 * pointsBuf, int maxBufSize, int * bufSize);

NEVector3 getNormalizedVector(NEVector3  original);

NEVector3 getProjectedPointInLine(NELine line, NEVector3 otherPoint);

bool shoudTrimPoint(NEVector3 point, NEFrustum frustum);

#ifdef __cplusplus
}
#endif

#endif /* NEMath_h */
