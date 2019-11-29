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

NEMatrix3 makeRotationMatrix(NEVector3 rotationAxis, float radians){
   NEVector3 v = getNormalizedVector(rotationAxis);
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
float vectorDotProduct(NEVector3 vec0,NEVector3 vec1){
    return vec0.x * vec1.x + vec0.y * vec1.y + vec0.z * vec1.z;
}

///||a||
float vectorMagnitude(NEVector3 vec){
    return sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
}

float getAngleBetweenVectors(NEVector3 vec0,NEVector3 vec1){
    float result = vectorDotProduct(vec0, vec1) /(vectorMagnitude(vec0) * vectorMagnitude(vec1));
    if (result < -1) {
        result = -1;
    } else if (result > 1) {
        result = 1;
    }
    float angle = acosf(result);
    return  angle;
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

NEVector3 reverseVector(NEVector3 vec){
    return NEVector3Make(- vec.x, - vec.y, - vec.z);
}

NEVector3 translationByVector(NEVector3 aPoint, NEVector3 translationVector){
    return NEVector3Make(aPoint.x + translationVector.x, aPoint.y + translationVector.y, aPoint.z + translationVector.z);
}

#define FLOAT_EQUAL(a, b) (fabs(a - b) <= 0.0000002)

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
    NEMatrix4 rotm =
    NEMatrix4Make(/*row0*/
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
    
    NEVector4 point4 = NEVector4MakeWithVector3(pointInCameraSpace, 1);
    
    NEVector4 res = NEMatrix4MultiplyVector4(rotm, point4);
    
    NEVector3 res3 = NEVector3Make(res.x / res.w, res.y / res.w, res.z / res.w);
    
    return res3;
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
    
    CGFloat step = 0.001;
    
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

NEVector3 getNormalizedVector(NEVector3  original){
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
    
    NEVector3 normVec = getNormalizedVector(line.vector);
    
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
