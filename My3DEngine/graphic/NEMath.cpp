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

NEVector4 NEVector4Make(float x, float y, float z, float w){
    NEVector4 v = { x, y, z, w };
    return v;
}

//#define NEVector3Make(x, y, z) GLKVector3Make(x, y, z)

NEVector3 NEVector3Make(float x, float y, float z){
    NEVector3 v = { x, y, z};
    return v;
}

//#define NEVector2Make(x, y) GLKVector2Make(x, y)
NEVector2 NEVector2Make(float x, float y){
    NEVector2 v = { x, y};
    return v;
}

//#define NEVector4MakeWithVector3(v, a) GLKVector4MakeWithVector3(v, a)

NEVector4 NEVector4MakeWithVector3(NEVector3 vector, float w){
    NEVector4 v = { vector.v[0], vector.v[1], vector.v[2], w };
    return v;
}

//#define NEMatrix3MultiplyVector3(m, v) GLKMatrix3MultiplyVector3(rotationMatrix, aPoint);
NEVector3 NEMatrix3MultiplyVector3(NEMatrix3 matrixLeft, NEVector3 vectorRight)
{
    NEVector3 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[3] * vectorRight.v[1] + matrixLeft.m[6] * vectorRight.v[2],
                     matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[7] * vectorRight.v[2],
                     matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] };
    return v;
}

//#define NEMatrix4MultiplyVector4(m, v)  GLKMatrix4MultiplyVector4(m, v)
NEVector4 NEMatrix4MultiplyVector4(NEMatrix4 matrixLeft, NEVector4 vectorRight)
{

    NEVector4 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] + matrixLeft.m[12] * vectorRight.v[3],
                     matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[9] * vectorRight.v[2] + matrixLeft.m[13] * vectorRight.v[3],
                     matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[6] * vectorRight.v[1] + matrixLeft.m[10] * vectorRight.v[2] + matrixLeft.m[14] * vectorRight.v[3],
                     matrixLeft.m[3] * vectorRight.v[0] + matrixLeft.m[7] * vectorRight.v[1] + matrixLeft.m[11] * vectorRight.v[2] + matrixLeft.m[15] * vectorRight.v[3] };
    return v;
}

NEVector3 NEVector3Add(NEVector3 vectorLeft, NEVector3 vectorRight)
{
    NEVector3 v = { vectorLeft.v[0] + vectorRight.v[0],
                     vectorLeft.v[1] + vectorRight.v[1],
                     vectorLeft.v[2] + vectorRight.v[2] };
    return v;
}

//#define NEVector2Subtract(a, b) GLKVector2Subtract(a, b)
NEVector2 NEVector2Subtract(NEVector2 vectorLeft, NEVector2 vectorRight)
{
    NEVector2 v = { vectorLeft.v[0] - vectorRight.v[0],
                     vectorLeft.v[1] - vectorRight.v[1] };
    return v;
}

//#define NEVector3Subtract(a, b) GLKVector3Subtract(a, b)
NEVector3 NEVector3Subtract(NEVector3 vectorLeft, NEVector3 vectorRight)
{
    NEVector3 v = { vectorLeft.v[0] - vectorRight.v[0],
                     vectorLeft.v[1] - vectorRight.v[1],
                     vectorLeft.v[2] - vectorRight.v[2] };
    return v;
}

//#define NEMatrix4Make(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) GLKMatrix4Make(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
NEMatrix4 NEMatrix4Make(float m00, float m01, float m02, float m03,
                                            float m10, float m11, float m12, float m13,
                                            float m20, float m21, float m22, float m23,
                                            float m30, float m31, float m32, float m33)
{
    NEMatrix4 m = { m00, m01, m02, m03,
                     m10, m11, m12, m13,
                     m20, m21, m22, m23,
                     m30, m31, m32, m33 };
    return m;
}

NEMatrix3 makeRotationMatrix(NEVector3 rotationAxis, float radians){
   NEVector3 v = NENormalize(rotationAxis);
   float cos = cosf(radians);
   float cosp = 1.0f - cos;
   float sin = sinf(radians);
   
   NEMatrix3 m = { cos + cosp * v.v[0] * v.v[0],
                    cosp * v.v[0] * v.v[1] + v.v[2] * sin,
                    cosp * v.v[0] * v.v[2] - v.v[1] * sin,

                    cosp * v.v[0] * v.v[1] - v.v[2] * sin,
                    cos + cosp * v.v[1] * v.v[1],
                    cosp * v.v[1] * v.v[2] + v.v[0] * sin,

                    cosp * v.v[0] * v.v[2] + v.v[1] * sin,
                    cosp * v.v[1] * v.v[2] - v.v[0] * sin,
                    cos + cosp * v.v[2] * v.v[2] };
   
   return m;
}

///a*b
float NEVector3DotProduct(NEVector3 vec0,NEVector3 vec1){
    return vec0.x * vec1.x + vec0.y * vec1.y + vec0.z * vec1.z;
}

///||a||
float vectorMagnitude(NEVector3 vec){
    return sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
}

float getAngleBetweenVectors(NEVector3 vec0,NEVector3 vec1){
    float result = NEVector3DotProduct(vec0, vec1) /(vectorMagnitude(vec0) * vectorMagnitude(vec1));
    if (result < -1) {
        result = -1;
    } else if (result > 1) {
        result = 1;
    }
    float angle = acosf(result);
    return  angle;
}

float NEVector3Length(NEVector3 vector)
{
    return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1] + vector.v[2] * vector.v[2]);
}

float NEVector2Length(NEVector2 vector)
{
    return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1]);
}

float NEVector2Distance(NEVector2 vectorStart, NEVector2 vectorEnd)
{
    return NEVector2Length(NEVector2Subtract(vectorEnd, vectorStart));
}

float NEVector3Distance(NEVector3 vectorStart, NEVector3 vectorEnd)
{
    return NEVector3Length(NEVector3Subtract(vectorEnd, vectorStart));
}

NEVector3 rotationByMatrix(NEVector3 aPoint, NEMatrix3 rotationMatrix){
    return NEMatrix3MultiplyVector3(rotationMatrix, aPoint);
}

NEVector3 rotationByAngle(NEVector3 aPoint, NEVector3 rotationAxis, float angle){
    NEMatrix3 rotm = makeRotationMatrix(rotationAxis, angle);
    NEVector3 ret = rotationByMatrix(aPoint, rotm);
    return ret;
}

NEVector3 rotationByAngleAroundLine(NEVector3 aPoint, NELine line, float angle){
    NEVector3 pointTrans = NEVector3Make(aPoint.x - line.pointInLine.x, aPoint.y - line.pointInLine.y, aPoint.z - line.pointInLine.z);
    
    NEVector3 rotated = rotationByAngle(pointTrans, line.vector, angle);
    NEVector3 ret = NEVector3Make(rotated.x + line.pointInLine.x, rotated.y + line.pointInLine.y, rotated.z + line.pointInLine.z);
    return ret;
}

NEVector3 rotationByAngleAroundLine2(NEVector3 aPoint, NEVector3 lineVec, NEVector3 aPointInLine, float angle){
    NELine line;
    line.pointInLine = aPointInLine;
    line.vector = lineVec;
    
    return rotationByAngleAroundLine(aPoint, line, angle);
}

NEVector3 vectorCrossProduct(NEVector3 vec0, NEVector3 vec1){
    NEVector3 cv = {
        vec0.y * vec1.z - vec0.z * vec1.y,
        vec0.z * vec1.x - vec0.x * vec1.z,
        vec0.x * vec1.y - vec0.y * vec1.x
    };
    return cv;
}

NEVector3 NEVector3Negate(NEVector3 vec){
    return NEVector3Make(- vec.x, - vec.y, - vec.z);
}

NEVector3 translationByVector(NEVector3 aPoint, NEVector3 translationVector){
    return NEVector3Make(aPoint.x + translationVector.x, aPoint.y + translationVector.y, aPoint.z + translationVector.z);
}

#define FLOAT_EQUAL(a, b) (fabs(a - b) <= 0.0000002)

void NEGetCoordConvertionRotationMatrixes(NEVector3 coordOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis, NEMatrix3 * outRotationZMatrix, NEMatrix3 * outRotationYMatrix){
    //now rotate z axis
    NEVector3 rotAxis = vectorCrossProduct(coordZAxis, NEVector3Make(0, 0, 1.));
    
    float rotZAngle = getAngleBetweenVectors(NEVector3Make(0, 0, 1.), coordZAxis);
    
    *outRotationZMatrix = makeRotationMatrix(rotAxis, rotZAngle);
    
    NEVector3 newCoordYAxis = rotationByMatrix(coordYAxis, *outRotationZMatrix);
    //angle between coordY axis and world y axis
    float rotYAngle = getAngleBetweenVectors(NEVector3Make(0, 1., 0), newCoordYAxis);
    
    if (!FLOAT_EQUAL(rotYAngle, M_PI)) {
        NEVector3 rotYAxis = vectorCrossProduct(newCoordYAxis, NEVector3Make(0, 1., 0));
        *outRotationYMatrix = makeRotationMatrix(rotYAxis, rotYAngle);
        
    } else {
        //when angle is 180, the vector cross result is a plane rather than a vector, so handle otherwise
        NEVector3 rotYAxis = NEVector3Make(0, 0, 1);
        *outRotationYMatrix = makeRotationMatrix(rotYAxis, rotYAngle);
    }
}

NEVector3 NEPostionConvertFromOriginalCoordSystem(NEVector3 targetOldPosition, NEVector3 coordOrigin, NEMatrix3 *rotationZMatrix, NEMatrix3 *rotationYMatrix){
    NEVector3 newPosition = translationByVector(targetOldPosition, NEVector3Negate(coordOrigin));
    
    newPosition = rotationByMatrix(newPosition, *rotationZMatrix);
    
    newPosition = rotationByMatrix(newPosition, *rotationYMatrix);
    
    return newPosition;
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
    NEVector3 newPosition = translationByVector(targetOldPosition, NEVector3Negate(coordOrigin));
    
    //now rotate z axis
    NEVector3 rotAxis = vectorCrossProduct(coordZAxis, NEVector3Make(0, 0, 1.));
    
    float rotZAngle = getAngleBetweenVectors(NEVector3Make(0, 0, 1.), coordZAxis);
    
    NEMatrix3 rotZMat = makeRotationMatrix(rotAxis, rotZAngle);
    
    newPosition = rotationByMatrix(newPosition, rotZMat);
    
    //now that we have rotated z axis, time to rotate y axis
    NEVector3 newCoordYAxis = rotationByMatrix(coordYAxis, rotZMat);
    //angle between coordY axis and world y axis
    float rotYAngle = getAngleBetweenVectors(NEVector3Make(0, 1., 0), newCoordYAxis);
    
    if (!FLOAT_EQUAL(rotYAngle, M_PI)) {
        NEVector3 rotYAxis = vectorCrossProduct(newCoordYAxis, NEVector3Make(0, 1., 0));
        NEMatrix3 rotYMat = makeRotationMatrix(rotYAxis, rotYAngle);
        newPosition = rotationByMatrix(newPosition, rotYMat);
    } else {
        //when angle is 180, the vector cross result is a plane rather than a vector, so handle otherwise
        NEVector3 rotYAxis = NEVector3Make(0, 0, 1);
        NEMatrix3 rotYMat = makeRotationMatrix(rotYAxis, rotYAngle);
        newPosition = rotationByMatrix(newPosition, rotYMat);
    }
    
    return newPosition;
}

NEVector3 getPositionInCameraCoordinateSystem(NEVector3 worldPosition, NEVector3 cameraPositionInWorld, NEVector3 cameraLookAt, NEVector3 cameraYAxisInWorld){
    NEVector3 pos = convertPositionFromOriginalCoordSystem(worldPosition, cameraPositionInWorld, cameraLookAt, cameraYAxisInWorld);
    
    //rotate it by 180 degrees around y axis to turn into eye space (in accordance with opengl frustum)
    NEVector3 ret = { - pos.x, pos.y, - pos.z};
    return ret;
}

bool shouldTrimPoint(NEVector3 point, NEFrustum frustum){
    if (point.z > - frustum.near || point.z < - frustum.far) {
        return true;
    }
    
    return false;
}

int testPointToLine2d(NEVector2 point, NEVector2 start, NEVector2 end){
//    NEVector2 v0 = NEVector2Subtract(point, start);
//    NEVector2 v1 = NEVector2Subtract(end, start);
    
//    return v0.x * v1.y - v1.x * v0.y;
    
    float v0dx = point.x - start.x;
    float v0dy = point.y - start.y;
    
    float v1dx = end.x - start.x;
    float v1dy = end.y - start.y;
    
    return v0dx * v1dy - v1dx * v0dy;
}

bool pointInsizeTriangle(NEVector2 point, NEVector2 p0, NEVector2 p1, NEVector2 p2){
    int t0 = testPointToLine2d(point, p0, p1);
    int t1 = testPointToLine2d(point, p1, p2);
    int t2 = testPointToLine2d(point, p2, p0);
    
    if ((t0 >= 0 && t1 >= 0 && t2 >= 0) ||
    (t0 <= 0 && t1 <= 0 && t2 <= 0)){
        return true;
    }
    
    return false;
}

NEVector3 getPointInPlane(float x, float y, NEVector3 normal, NEVector3 aPointInPlane){
    if (normal.z == 0) {
        return NEVector3Make(x, y, aPointInPlane.z);
    }
    
    float vx = x - aPointInPlane.x;
    float vy = y - aPointInPlane.y;
    
    float z = - (vx * normal.x + vy * normal.y) / normal.z + aPointInPlane.z;
    
    return NEVector3Make(x, y, z);
}

NEVector3 perspetiveProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum){
    /*
    float m00 = frustum.near / frustum.r;
    
    float m11 = frustum.near / frustum.t;
    
    float m22 = - (frustum.far + frustum.near) / (frustum.far - frustum.near);
    
    float m23 = - 2 * frustum.far * frustum.near / (frustum.far - frustum.near);
    
    float zn = (m22 * pointInCameraSpace.z + m23)/(-pointInCameraSpace.z);
     */
    
    NEMatrix4 rotm =
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
    
    NEVector4 point4 = NEVector4MakeWithVector3(pointInCameraSpace, 1);
    
    NEVector4 res = NEMatrix4MultiplyVector4(rotm, point4);

    NEVector3 res3 = NEVector3Make(res.x / res.w, res.y / res.w, res.z / res.w);
    
    return res3;
}

NEVector3 invertPerspetiveProject(NEVector3 pointInEyeSpace, NEFrustum frustum){
    float m00 = frustum.near / frustum.r;
    
    float m11 = frustum.near / frustum.t;
    
    float m22 = - (frustum.far + frustum.near) / (frustum.far - frustum.near);
    
    float m23 = - 2 * frustum.far * frustum.near / (frustum.far - frustum.near);
    
    float ze = -m23 / (pointInEyeSpace.z + m22);
    float xe = pointInEyeSpace.x * (-ze) / m00;
    float ye = pointInEyeSpace.y * (-ze) / m11;
    
    return NEVector3Make(xe, ye, ze);
}

NEVector3 orhographicProjectPoint(NEVector3 pointInCameraSpace, NEFrustum frustum){
    NEMatrix4 rotm =
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
    
    NEVector4 point4 = NEVector4MakeWithVector3(pointInCameraSpace, 1);
    
    NEVector4 res = NEMatrix4MultiplyVector4(rotm, point4);

    NEVector3 res3 = NEVector3Make(res.x / res.w, res.y / res.w, res.z / res.w);
    
    return res3;
}

NEVector3 invertOrthographicsProject(NEVector3 pointInEyeSpace, NEFrustum frustum){
    float m00 = 1 / frustum.r;
    
    float m11 = 1 / frustum.t;
    
    float m22 = - 2 / (frustum.far - frustum.near);
            
    float m23 = - (frustum.far + frustum.near) / (frustum.far - frustum.near);
    
    float ze = (pointInEyeSpace.z - m23) / m22;
    float xe = pointInEyeSpace.x / m00;
    float ye = pointInEyeSpace.y / m11;
    
    return NEVector3Make(xe, ye, ze);
}

NEVector3 getVerticalVec(NEVector3 vec, float *px, float *py, float *pz){
    if (!px) {
        if (vec.x != 0) {
            float x = - (vec.y * *py + vec.z * *pz)/vec.x;
            return NEVector3Make(x, *py, *pz);
        } else {
            return NEVector3Make(1, 0, 0);
        }
    } else if (!py) {
        if (vec.y != 0) {
            float y = - (vec.x * *px + vec.z * *pz)/vec.y;
            return NEVector3Make(*px, y, *pz);
        } else {
            return NEVector3Make(0, 1, 0);
        }
    } else if (!pz) {
       if (vec.z != 0) {
           float z = - (vec.x * *px + vec.y * *py)/vec.z;
           return NEVector3Make(*px, *py, z);
       } else {
           return NEVector3Make(0, 0, 1);
       }
    } else {
        return NEVector3Make(0, 0, 0);
    }
}

void safeIncreaseSize(NEVector3 * pointsBuf, int maxBufSize, int * bufSize, float x, float y, float z){
    if (*bufSize < maxBufSize) {
        pointsBuf[*bufSize] = NEVector3Make(x, y, z);
        *bufSize = *bufSize + 1;
    }
}

NE_RESULT getPointsArrayInLine(NEVector3 start, NEVector3 end, NEVector3 * pointsBuf, int maxBufSize, int * bufSize){
    *bufSize = 0;
    
    float step = 0.001;
    
    if (start.x == end.x && start.y != end.y) {
        float k_yz = (end.z - start.z)/( end.y - start.y);
        for (float s =  MIN(start.y, end.y) ; s < MAX(start.y, end.y) ; s += step) {
            float z = k_yz * ( s - start.y) + start.z;
//            if (*bufSize <= maxBufSize) {
//                pointsBuf[*bufSize] = NEVector3Make(start.x, s, z);
//                *bufSize = *bufSize + 1;
//            }
            
            safeIncreaseSize(pointsBuf, maxBufSize, bufSize, start.x, s, z);
        }
        
        float max_y = MAX(start.y, end.y);
        
//        if (*bufSize <= maxBufSize) {
//            pointsBuf[*bufSize] = NEVector3Make(start.x, max_y, max_y == start.y?start.z:end.z);
//            *bufSize = *bufSize + 1;
//        }
        
        safeIncreaseSize(pointsBuf, maxBufSize, bufSize, start.x, max_y, max_y == start.y?start.z:end.z);
        
        return NE_RESULT_OK;
    }
    
    if (start.y == end.y && start.x != end.x) {
        float k_xz = (end.z - start.z)/( end.x - start.x);
        for (float s =  MIN(start.x, end.x) ; s < MAX(start.x, end.x) ; s += step) {
            float z = k_xz * ( s - start.x) + start.z;
            
//            if (*bufSize <= maxBufSize) {
//                pointsBuf[*bufSize] = NEVector3Make(s, start.y, z);
//                *bufSize = *bufSize + 1;
//            }
            
            safeIncreaseSize(pointsBuf, maxBufSize, bufSize, s, start.y, z);
        }
        
        float max_x = MAX(start.x, end.x);
        
//        if (*bufSize <= maxBufSize) {
//            pointsBuf[*bufSize] = NEVector3Make(max_x, start.y, max_x == start.x?start.z:end.z);
//            *bufSize = *bufSize + 1;
//        }
        
        safeIncreaseSize(pointsBuf, maxBufSize, bufSize, max_x, start.y, max_x == start.x?start.z:end.z);
        
        return NE_RESULT_OK;
    }
    
    if (start.x != end.x && start.y != end.y) {
        
        if (fabs(end.x - start.x) > fabs(end.y - start.y)) {
            //always use the longest diff of coord
            float k = (end.y - start.y)/( end.x - start.x);
            float k_xz = (end.z - start.z)/( end.x - start.x);
            
            for (float s =  MIN(start.x, end.x) ; s < MAX(start.x, end.x) ; s += step) {
                float y = k * ( s - start.x) + start.y;
                float z = k_xz * ( s - start.x) + start.z;
                
//                if (*bufSize <= maxBufSize) {
//                    pointsBuf[*bufSize] = NEVector3Make(s, y, z);
//                    *bufSize = *bufSize + 1;
//                }
                
                safeIncreaseSize(pointsBuf, maxBufSize, bufSize, s, y, z);
            }
            
            if (start. x < end.x) {
//                if (*bufSize <= maxBufSize) {
//                    pointsBuf[*bufSize] = NEVector3Make(end.x, end.y, end.z);
//                    *bufSize = *bufSize + 1;
//                }
                
                safeIncreaseSize(pointsBuf, maxBufSize, bufSize, end.x, end.y, end.z);
            } else {
//                if (*bufSize <= maxBufSize) {
//                    pointsBuf[*bufSize] = NEVector3Make(start.x, start.y, start.z);
//                    *bufSize = *bufSize + 1;
//                }
                
                safeIncreaseSize(pointsBuf, maxBufSize, bufSize, start.x, start.y, start.z);
            }
        } else {
            float k = ( end.x - start.x) / (end.y - start.y);
            float k_yz = (end.z - start.z)/( end.y - start.y);
            
            for (float s =  MIN(start.y, end.y) ; s < MAX(start.y, end.y) ; s += step) {
                float x = k * ( s - start.y) + start.x;
                float z = k_yz * ( s - start.y) + start.z;
                
//                if (*bufSize <= maxBufSize) {
//                    pointsBuf[*bufSize] = NEVector3Make(x, s, z);
//                    *bufSize = *bufSize + 1;
//                }
                
                safeIncreaseSize(pointsBuf, maxBufSize, bufSize, x, s, z);
            }
            
            if (start. x < end.x) {
//                pointsBuf[*bufSize] = NEVector3Make(end.x, end.y, end.z);
//                *bufSize = *bufSize + 1;
                
                safeIncreaseSize(pointsBuf, maxBufSize, bufSize, end.x, end.y, end.z);
            } else {
//                pointsBuf[*bufSize] = NEVector3Make(start.x, start.y, start.z);
//                *bufSize = *bufSize + 1;
                
                safeIncreaseSize(pointsBuf, maxBufSize, bufSize, start.x, start.y, start.z);
            }
        }
        
        
        return NE_RESULT_OK;
    }
    
    return NE_RESULT_FAIL;
    
}

NEVector3 NENormalize(NEVector3  original){
    NEVector3 n;
    float d = sqrtf(powf(original.x, 2) + powf(original.y, 2) + powf(original.z, 2));
    n.x = original.x/d;
    n.y = original.y/d;
    n.z = original.z/d;
    return n;
}

NEVector3 getPlaneNormal(NEVector3 p0, NEVector3 p1, NEVector3 p2){
    NEVector3 v0 = NEVector3Subtract(p1, p0);
    NEVector3 v1 = NEVector3Subtract(p2, p0);
    
    return vectorCrossProduct(v0, v1);
}

NEVector3 getProjectedPointInLine(NELine line, NEVector3 otherPoint){
    NEVector3 pointTrans = NEVector3Make(otherPoint.x - line.pointInLine.x, otherPoint.y - line.pointInLine.y, otherPoint.z - line.pointInLine.z);
    
    NEVector3 normVec = NENormalize(line.vector);
    
    float deno = powf(normVec.x, 2) + powf(normVec.y, 2) + powf(normVec.z, 2);
    
    if (deno == 0) {
        return line.pointInLine;
    }
    
    float numer = pointTrans.x * normVec.x + pointTrans.y* normVec.y + pointTrans.z * normVec.z;
    
    float d = numer / deno;
    
    NEVector3 point = NEVector3Make( d * normVec.x + line.pointInLine.x,
                                     d * normVec.y + line.pointInLine.y,
                                     d * normVec.z + line.pointInLine.z);
    
    return point;
}

NEVector3 NEVector3MultiplyScalar(NEVector3 vector, float value){
    NEVector3 v = { vector.v[0] * value,
                     vector.v[1] * value,
                     vector.v[2] * value };
    return v;
}

float getInterpolatedValueForTriangle2(NEVector2 p, NEVector2 v0, NEVector2 v1, NEVector2 v2, float intensity0, float intensity1, float intensity2){
    
    bool canJoint = true;
    
    NEVector2 joint = getJointPoint(v0, v1, v2, p, &canJoint);
    
    float lengh01 = NEVector2Distance(v0, v1);
    float lengh0j = NEVector2Distance(v0, joint);
    
    float lengh2p = NEVector2Distance(v2, p);
    float lengh2j = NEVector2Distance(v2, joint);
    
    if (lengh01 == 0 || lengh2j == 0) {
        return (intensity0 + intensity1 + intensity2)/3;
    }
    
    float intensityJ = (intensity1 - intensity0) * lengh0j/lengh01 + intensity0;
    
    
    
    float intensityX = (intensityJ - intensity2) * lengh2p/lengh2j + intensity2;
    
    return intensityX;
}

float getInterpolatedValueForTriangle3(NEVector3 point, NEVector3 vert0, NEVector3 vert1, NEVector3 vert2, float intensity0, float intensity1, float intensity2){
    NEVector2 p = NEVector2Make(point.x, point.y);
    
    NEVector2 v0 = NEVector2Make(vert0.x, vert0.y);
    NEVector2 v1 = NEVector2Make(vert1.x, vert1.y);
    NEVector2 v2 = NEVector2Make(vert2.x, vert2.y);
    
    return getInterpolatedValueForTriangle2(p, v0, v1, v2, intensity0, intensity1, intensity2);
}

NEVector2 getJointPoint(NEVector2 line1Start, NEVector2 line1end, NEVector2 line2Start, NEVector2 line2end, bool *canJoint){
    if (line1end.x == line1Start.x) {
        if (line2Start.x == line2end.x) {
            *canJoint = false;
            return NEVector2Make(0, 0);
        } else {
            float m = (line2end.y - line2Start.y)/(line2end.x - line2Start.x);
            float x = line1end.x;
            float y = m * (x  - line2Start.x) + line2Start.y ;
            return NEVector2Make(x, y);
        }
    }
    
    if (line2end.x == line2Start.x) {
        if (line1Start.x == line1end.x) {
            *canJoint = false;
            return NEVector2Make(0, 0);
        } else {
            float k = (line1end.y - line1Start.y)/(line1end.x - line1Start.x);
            float x = line2end.x;
            float y = k * (x  - line1Start.x) + line1Start.y ;
            return NEVector2Make(x, y);
        }
    }
    
    float k = (line1end.y - line1Start.y)/(line1end.x - line1Start.x);
    
    float m = (line2end.y - line2Start.y)/(line2end.x - line2Start.x);
    
    if (k == m) {
        *canJoint = false;
        return NEVector2Make(0, 0);
    }
    
    float x = (k * line1Start.x + line2Start.y  - m * line2Start.x - line1Start.y)/(k - m);
    
    float y = k * ( x - line1Start.x) + line1Start.y ;
    
    *canJoint = true;
    return NEVector2Make(x, y);
    
}

NEVector3 NEConvertVectorToCoordSystem(NEVector3 aVectorInOldSystem, NEVector3 targetSystemOrigin, NEVector3 coordZAxis, NEVector3 coordYAxis){
    NEVector3 vectorT = convertPositionFromOriginalCoordSystem(aVectorInOldSystem, targetSystemOrigin, coordZAxis, coordYAxis);
    return NEVector3Subtract(vectorT, targetSystemOrigin);
}

float saturate(float val){
    if (val < 0) {
        return 0;
    }
    else if (val > 1) {
        return 1;
    }
    else {
        return val;
    }
}

NELong colorMul(NELong color, float f){
    NELong r = (color >>16) & 0xff;
    NELong g = (color >>8) & 0xff;
    NELong b = color & 0xff;
    
    r = r * f ;
    g = g * f ;
    b = b * f ;
    
//    if (intensity < 0.01){
//        int k = 0;
//    }
    
    return (r ) <<16 | (g ) <<8 | (b );
}

NELong colorAdd(NELong color, NELong color2){
    NELong r = (color >>16) & 0xff;
    NELong g = (color >>8) & 0xff;
    NELong b = color & 0xff;
        
    NELong r2 = (color2 >>16) & 0xff;
    NELong g2 = (color2 >>8) & 0xff;
    NELong b2 = color2 & 0xff;
    
    NELong r0 = r + r2;
    NELong g0 = g + g2;
    NELong b0 = b + b2;
    
    if (r0 > 0xff) {
        r0 = 0xff;
    }
    
    if (g0 > 0xff) {
        g0 = 0xff;
    }
    
    if (b0 > 0xff) {
        b0 = 0xff;
    }
        
    return r0 <<16 | g0 <<8 | b0;
}
