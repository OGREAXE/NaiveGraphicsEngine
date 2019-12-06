//
//  NECoreGraphicsDevice.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NECoreGraphicsDevice.hpp"
#include "NEDisplayDevice.hpp"
#include "NECommon.h"

#import <UIKit/UIKit.h>

void NECoreGraphicsDevice::presentRenderBuffer(RenderBufferType *renderBuffer, int renderBufferSize, int mode, void *extra){
    CGFloat fillWidth = 1./COORD_AMPLIFY_FACTOR;
        
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    long lastColor = 0;
    for (int i = 0; i < renderBufferSize; i++) {
        RenderBufferType renderValue = renderBuffer[i];
        
//        int x = renderValue & 0xffff;
//        int y = (renderValue >> 16) & 0xffff;
//        long renderColor = (renderValue >> 32);
        
        int x = RenderValueGetX(renderValue);
        int y = RenderValueGetY(renderValue);
        long renderColor = RenderValueGetColor(renderValue);
        
        CGRect fillRect = CGRectMake(x / COORD_AMPLIFY_FACTOR - fillWidth/2, y / COORD_AMPLIFY_FACTOR - fillWidth/2, fillWidth, fillWidth);
        
        if (renderColor != lastColor) {
            lastColor = renderColor;
//            CGContextSetFillColorWithColor(context, HEXRGBCOLOR(renderColor).CGColor);
        }
        CGContextSetFillColorWithColor(context, HEXRGBCOLOR(renderColor).CGColor);
        CGContextFillRect(context, fillRect);
    }
}

