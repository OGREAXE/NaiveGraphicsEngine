//
//  NECoreGraphicsTextureProvider.cpp
//  NEDemo_Mac
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/11.
//  Copyright © 2019 Liang,Zhiyuan(GIS). All rights reserved.
//

#include "NECoreGraphicsTextureProvider.hpp"
#import <UIKit/UIKit.h>

long NECoreGraphicsTextureProvider::readColorFromTexture(int textureIndex, float u, float v){
    if (textureIndex >= _textures.size()) {
//        NSLog(@"texture index out of range");
        return 0;
    }
    
    if(u < 0 || u > 1 || v < 0 || v > 1 ){
        return 0;
    }
    
    NETexture &tex = _textures[textureIndex];
    
    if (!_imageBuf[textureIndex]) {
        NSString *filename = [NSString stringWithUTF8String:tex.path.c_str()];
        
        NSString * path = [[NSBundle mainBundle] pathForResource:[filename stringByDeletingPathExtension] ofType:[filename pathExtension]];
        
        UIImage * image = [[UIImage alloc] initWithContentsOfFile:path];
        
        tex.width = image.size.width;
        tex.height = image.size.height;

        CGImageRef cgImage = [image CGImage];
        CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        NEImageBufferPointer buffer =  (NEImageBufferPointer)CFDataGetBytePtr(data);
        
//        long length = CFDataGetLength(data);
        
        _imageBuf[textureIndex] = buffer;
    }
    
#define COLOR_CHAN_CNT 4
    
    long xIdx = tex.width * u;
    long yIdx = tex.height * v;
    
    long index = COLOR_CHAN_CNT * ( tex.width * yIdx + xIdx);
    
    long r = _imageBuf[textureIndex][index];
    long g = _imageBuf[textureIndex][index + 1];
    long b = _imageBuf[textureIndex][index + 2];
    long a = _imageBuf[textureIndex][index + 3];
    
//    return r<<24|g<<16|b<<8|a;
    return r<<16|g<<8|b;
}
