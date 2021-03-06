//
//  NEMath.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEMath_h
#define NEMath_h

//#import <GLKit/GLKit.h>

#ifdef __cplusplus
extern "C"{
#endif

#define NE_RESULT int
#define NE_RESULT_FAIL 1
#define NE_RESULT_OK 0

typedef unsigned long NELong;

///vector definitions
//typedef GLKVector2 NEVector2;
union _NEVector2
{
    struct { float x, y; };
    struct { float s, t; };
    float v[2];
} __attribute__((aligned(8)));
typedef union _NEVector2 NEVector2;

//typedef GLKVector3 NEVector3;
union _NEVector3
{
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float s, t, p; };
    float v[3];
};
typedef union _NEVector3 NEVector3;

//typedef GLKVector4 NEVector4;
union _NEVector4
{
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { float s, t, p, q; };
    float v[4];
} __attribute__((aligned(16)));
typedef union _NEVector4 NEVector4;

///matrix definitions
//typedef GLKMatrix3 NEMatrix3;
union _NEMatrix3
{
    struct
    {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
    };
    float m[9];
};
typedef union _NEMatrix3 NEMatrix3;

//typedef GLKMatrix4 NEMatrix4;
union _NEMatrix4
{
    struct
    {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
    float m[16];
} __attribute__((aligned(16)));
typedef union _NEMatrix4 NEMatrix4;

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

#if !defined(MIN)
    #define MIN(A,B)    ((A) < (B) ? (A) : (B))
#endif

#if !defined(MAX)
    #define MAX(A,B)    ((A) > (B) ? (A) : (B))
#endif

NEVector2 NEVector2Make(float x, float y);

NEVector3 NEVector3Make(float x, float y, float z);

NEVector4 NEVector4Make(float x, float y, float z, float w);

NEVector4 NEVector4MakeWithVector3(NEVector3 vector, float w);

NEVector3 NEMatrix3MultiplyVector3(NEMatrix3 matrixLeft, NEVector3 vectorRight);

NEVector4 NEMatrix4MultiplyVector4(NEMatrix4 matrixLeft, NEVector4 vectorRight);

NEVector3 NEVector3Add(NEVector3 vectorLeft, NEVector3 vectorRight);

NEVector2 NEVector2Subtract(NEVector2 vectorLeft, NEVector2 vectorRight);

NEVector3 NEVector3Subtract(NEVector3 vectorLeft, NEVector3 vectorRight);

NEVector3 NEVector3MultiplyScalar(NEVector3 vector, float value);

NEMatrix4 NEMatrix4Make(float m00, float m01, float m02, float m03,
                                            float m10, float m11, float m12, float m13,
                                            float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33);

NEMatrix3 makeRotationMatrix(NEVector3 rotationAxis, float angle);

///a*b
float NEVector3DotProduct(NEVector3 vec0,NEVector3 vec1);

NEVector3 vectorCrossProduct(NEVector3 vec0, NEVector3 vec1);

///||a||
float vectorMagnitude(NEVector3 vec);


/// return [0 ; π]
/// @param vec0 <#vec0 description#>
/// @param vec1 <#vec1 description#>
float getAngleBetweenVectors(NEVector3 vec0,NEVector3 vec1);

float NEVector2Length(NEVector2 vector);

float NEVector3Length(NEVector3 vector);

float NEVector2Distance(NEVector2 vectorStart, NEVector2 vectorEnd);

float NEVector3Distance(NEVector3 vectorStart, NEVector3 vectorEnd);

NEVector3 rotationByAngle(NEVector3 aPoint, NEVector3 rotationAxis, float angle);

NEVector3 rotationByAngleAroundLine(NEVector3 aPoint, NELine line, float angle);

NEVector3 rotationByAngleAroundLine2(NEVector3 aPoint, NEVector3 lineVec, NEVector3 aPointInLine, float angle);

NEVector3 rotationByMatrix(NEVector3 aPoint, NEMatrix3 rotationMatrix);

NEVector3 NEVector3Negate(NEVector3 vec);

NEVector3 translationByVector(NEVector3 aPoint, NEVector3 translationVector);

void NEGetCoordConvertionRotationMatrixes(NEVector3 coordOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis, NEMatrix3 * outRotationZMatrix, NEMatrix3 * outRotationYMatrix);

NEVector3 convertPositionFromOriginalCoordSystem(NEVector3 targetOldPosition, NEVector3 coordOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis);

NEVector3 NEPostionConvertFromOriginalCoordSystem(NEVector3 targetOldPosition, NEVector3 coordOriginInWorld, NEMatrix3 *rotationZMatrix, NEMatrix3 *rotationYMatrix);

NEVector3 getPositionInCameraCoordinateSystem(NEVector3 worldPosition, NEVector3 cameraPositionInWorld, NEVector3 cameraLookAt, NEVector3 cameraYAxisInWorld);

NEVector3 perspetiveProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum);

NEVector3 invertPerspetiveProject(NEVector3 pointInEyeSpace, NEFrustum frustum);

NEVector3 orhographicProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum);

NEVector3 invertOrthographicsProject(NEVector3 pointInEyeSpace, NEFrustum frustum);

NEVector3 getVerticalVec(NEVector3 vec, float *x, float *y, float *z);

NE_RESULT getPointsArrayInLine(NEVector3 start, NEVector3 end, NEVector3 * pointsBuf, int maxBufSize, int * bufSize);

NEVector3 NENormalize(NEVector3  original);

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

NEVector2 getJointPoint(NEVector2 line1Start, NEVector2 line1end, NEVector2 line2Start, NEVector2 line2end, bool *canJoint);

float getInterpolatedValueForTriangle3(NEVector3 point, NEVector3 vert0, NEVector3 vert1, NEVector3 vert2, float intensity0, float intensity1, float intensity2);

float getInterpolatedValueForTriangle2(NEVector2 p, NEVector2 v0, NEVector2 v1, NEVector2 v2, float intensity0, float intensity1, float intensity2);

NEVector3 NEConvertVectorToCoordSystem(NEVector3 aVectorInOldSystem, NEVector3 targetSystemOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis);

float saturate(float val);

NELong colorMul(NELong color, float f);

NELong colorAdd(NELong color, NELong color2);

#ifdef __cplusplus
}
#endif

#endif /* NEMath_h */
