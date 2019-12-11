//
//  NEDepthBuffer.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/11/17.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "NEDepthBuffer.hpp"

using namespace std;

int NE_DEPTHBUFFER_OK = 0;
int NE_DEPTHBUFFER_PARAM_ERROR = 1;

void NEDepthBuffer::initMap(){
    m_depthInfoMap.resize(m_height);
    
    for (int i = 0; i < m_height; i++) {
        vector<DepthInfo> aRow(m_width);
        m_depthInfoMap[i] = aRow;
        
        for (int j = 0; j < m_width; j++) {
            //set to -2 'cause NDC range is (-1, 1)
            setZ(2, j, i);
        }
    }
}

int NEDepthBuffer::setInfo(DepthInfo & info, int x, int y){
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return NE_DEPTHBUFFER_PARAM_ERROR;
    }
    
    m_depthInfoMap[y][x] = info;
    return NE_DEPTHBUFFER_OK;
}


DepthInfo NEDepthBuffer::getInfo(int x, int y){
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return {-2, 0, 0};
    }
    
    return m_depthInfoMap[y][x];
}

int NEDepthBuffer::setZ(float zVal, int x, int y){
    DepthInfo info = {0, 0, 0};
    info.z = zVal;
    setInfo(info, x, y);
    
    return NE_DEPTHBUFFER_OK;
}

float NEDepthBuffer::getZ(int x, int y){
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return 0;
    }
    
    return m_depthInfoMap[y][x].z;
}
