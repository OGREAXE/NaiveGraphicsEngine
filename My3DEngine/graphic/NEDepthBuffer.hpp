//
//  NEDepthBuffer.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/11/17.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEDepthBuffer_hpp
#define NEDepthBuffer_hpp

#include <stdio.h>
#include <vector>

extern int NE_DEPTHBUFFER_OK;
extern int NE_DEPTHBUFFER_PARAM_ERROR;

typedef struct tagDepthInfo{
    float z;
    long color;
} DepthInfo;

class NEDepthBuffer {
    std::vector<std::vector<DepthInfo>> m_depthInfoMap; //2d z buffer map
    int m_width;
    int m_height;
    
    void initMap();
public:
    NEDepthBuffer():m_width(0), m_height(0){}
    
    NEDepthBuffer(int width, int height):m_width(width), m_height(height){ initMap();}
    
    int setZ(float zVal, int x, int y);
    
    int setInfo(DepthInfo & info, int x, int y);
    
    /// upon failure return something out of range (-1, 1), such as -2
    /// @param x <#x description#>
    /// @param y <#y description#>
    float getZ(int x, int y);
    
    DepthInfo getInfo(int x, int y);
    
    void clear(){m_depthInfoMap.clear(); m_width = 0; m_height = 0;}
    
    void resetSize(int width, int height) {m_width = width; m_height = height; initMap();}
    void resetSize() {m_depthInfoMap.clear(); initMap();}
    
    int getWidth() {return m_width;}
    int getHeight() {return m_height;}
};

#endif /* NEDepthBuffer_hpp */
