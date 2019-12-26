//
//  NEInterpolation.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/26.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEInterpolation_hpp
#define NEInterpolation_hpp

#include <stdio.h>
#include "NEMath.h"

#define NE_STATIC_INLINE static inline

typedef struct tagPosition2dWithPayload {
    NEVector2 vec;
    float payload[kPayloadMaxCount];
} Position2DWithPayload;

typedef struct tagNLine {
    Position2DWithPayload *start;
    Position2DWithPayload *end;
} NLine;

NE_STATIC_INLINE float getJointXByY(float y, NEVector2 &lstart, NEVector2 &lend){
    if(lstart.x == lend.x){
        return lstart.x;
    }
 
    float x = (y - lstart.y ) * (lend.x - lstart.x) / (lend.y - lstart.y) + lstart.x;
    
    return x;
}

NE_STATIC_INLINE bool getLinePostion(NLine & line0, NLine & line1, NLine & line2, NLine **_lineLeft, NLine **_lineRight, float scanLineY){
    NLine *lineLeft = NULL, *lineRight = NULL;
    
    if ((line0.start->vec.y - scanLineY) * (line0.end->vec.y - scanLineY) >= 0) {
        lineLeft = &line1;
        lineRight = &line2;
    } else if ((line1.start->vec.y - scanLineY) * (line1.end->vec.y - scanLineY) >= 0) {
        lineLeft = &line0;
        lineRight = &line2;
    } else if ((line2.start->vec.y - scanLineY) * (line2.end->vec.y - scanLineY) >= 0) {
        lineLeft = &line0;
        lineRight = &line1;
    }
    
    float jxl = getJointXByY(scanLineY, (*lineLeft).start->vec, (*lineLeft).end->vec);
    float jxr = getJointXByY(scanLineY, (*lineRight).start->vec, (*lineRight).end->vec);
    
    if (jxl > jxr) {
        NLine *temp = lineLeft;
        lineLeft = lineRight;
        lineRight = temp;
        
        float tmpJ = jxl;
        jxl = jxr;
        jxr = tmpJ;
    }
    
    *_lineLeft = lineLeft;
    *_lineRight = lineRight;
    
    return true;
}

NE_STATIC_INLINE bool getLineJointPoint(NLine &lineLeft, NLine &lineRight, Position2DWithPayload &jointLeft, Position2DWithPayload &jointRight,  float scanLineY){
    float jxl = getJointXByY(scanLineY, (lineLeft).start->vec, (lineLeft).end->vec);
    float jxr = getJointXByY(scanLineY, (lineRight).start->vec, (lineRight).end->vec);
    
    jointLeft.vec.x = jxl;
    jointLeft.vec.y = scanLineY;
    
    jointRight.vec.x = jxr;
    jointRight.vec.y = scanLineY;
    
    return true;
}

#endif /* NEInterpolation_hpp */
