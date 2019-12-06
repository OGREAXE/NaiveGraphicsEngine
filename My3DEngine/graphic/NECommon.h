//
//  NECommon.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NECommon_h
#define NECommon_h

#define NE_INLINE    static __inline__

#include "NEMath.h"
//#import "NEObject.h"
//#import "NECamera.h"

#define NELog(fmt, ...) printf(fmt, ##__VA_ARGS__)

#define NESafeDelete(p) if(p){delete p;}

typedef long long RenderBufferType;

#define COORD_AMPLIFY_FACTOR 1

////color
#define RGBACOLOR(r,g,b,a)    ([UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:(a)])
#define RGBCOLOR(r,g,b)       RGBACOLOR(r,g,b,1)
#define HEXRGBACOLOR(h,a)     RGBACOLOR(((h>>16)&0xFF), ((h>>8)&0xFF), (h&0xFF), a)
#define HEXRGBCOLOR(h)        HEXRGBACOLOR(h,1)

#endif /* MECommon_h */
