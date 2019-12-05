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

NEVector2 NEVector2Make(float x, float y);

NEVector3 NEVector3Make(float x, float y, float z);

NEVector4 NEVector4Make(float x, float y, float z, float w);

NEVector4 NEVector4MakeWithVector3(NEVector3 vector, float w);

NEVector3 NEMatrix3MultiplyVector3(NEMatrix3 matrixLeft, NEVector3 vectorRight);

NEVector4 NEMatrix4MultiplyVector4(NEMatrix4 matrixLeft, NEVector4 vectorRight);

NEVector2 NEVector2Subtract(NEVector2 vectorLeft, NEVector2 vectorRight);

NEVector3 NEVector3Subtract(NEVector3 vectorLeft, NEVector3 vectorRight);

NEMatrix4 NEMatrix4Make(float m00, float m01, float m02, float m03,
                                            float m10, float m11, float m12, float m13,
                                            float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33);

NEMatrix3 makeRotationMatrix(NEVector3 rotationAxis, float angle);

///a*b
float vectorDotProduct(NEVector3 vec0,NEVector3 vec1);

NEVector3 vectorCrossProduct(NEVector3 vec0, NEVector3 vec1);

///||a||
float vectorMagnitude(NEVector3 vec);


/// return [0 ; π]
/// @param vec0 <#vec0 description#>
/// @param vec1 <#vec1 description#>
float getAngleBetweenVectors(NEVector3 vec0,NEVector3 vec1);

NEVector3 rotationByAngle(NEVector3 aPoint, NEVector3 rotationAxis, float angle);

NEVector3 rotationByAngleAroundLine(NEVector3 aPoint, NELine line, float angle);

NEVector3 rotationByAngleAroundLine2(NEVector3 aPoint, NEVector3 lineVec, NEVector3 aPointInLine, float angle);

NEVector3 rotationByMatrix(NEVector3 aPoint, NEMatrix3 rotationMatrix);

NEVector3 reverseVector(NEVector3 vec);

NEVector3 translationByVector(NEVector3 aPoint, NEVector3 translationVector);

NEVector3 convertPositionFromOriginalCoordSystem(NEVector3 targetOldPosition, NEVector3 coordOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis);

NEVector3 getPositionInCameraCoordinateSystem(NEVector3 worldPosition, NEVector3 cameraPositionInWorld, NEVector3 cameraLookAt, NEVector3 cameraYAxisInWorld);

NEVector3 perspetiveProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum);

NEVector3 invertPerspetiveProject(NEVector3 pointInEyeSpace, NEFrustum frustum);

NEVector3 getVerticalVec(NEVector3 vec, float *x, float *y, float *z);

NE_RESULT getPointsArrayInLine(NEVector3 start, NEVector3 end, NEVector3 * pointsBuf, int maxBufSize, int * bufSize);

NEVector3 getNormalizedVector(NEVector3  original);

NEVector3 getProjectedPointInLine(NELine line, NEVector3 otherPoint);

bool shouldTrimPoint(NEVector3 point, NEFrustum frustum);


/// if result > 0 then point is to the right of line
/// if result == 0 then point is in the line
/// if result < 0 then point is to the left of line
/// @param point <#point description#>
/// @param start <#start description#>
/// @param end <#end description#>
inline int testPointToLine2d(NEVector2 point, NEVector2 start, NEVector2 end);

inline bool pointInsizeTriangle(NEVector2 point, NEVector2 p0, NEVector2 p1, NEVector2 p2);

NEVector3 getPlaneNormal(NEVector3 p0, NEVector3 p1, NEVector3 p2);

NEVector3 getPointInPlane(float x, float y, NEVector3 normal, NEVector3 aPointInPlane);

#ifdef __cplusplus
}
#endif

#endif /* NEMath_h */
