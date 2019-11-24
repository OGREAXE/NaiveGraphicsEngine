//
//  NEScreenRenderer.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEScreenRenderer_hpp
#define NEScreenRenderer_hpp

#include <stdio.h>
#include <functional>
#include <vector>
#include "NEMesh.h"
#include "NEDepthBuffer.hpp"

typedef struct tagNERenderPart{
    int x;
    int y;
    long color;
}NERenderPart;

class NEScreenRenderer {
    int m_width;
    int m_height;
    
public:
    
    bool geFaceRenderParts(NEFace &aFace, NEDepthBuffer &zBuffer, std::vector<NERenderPart> & renderParts);
    
    void setWidth(int width){ m_width = width;};
    void setHeight(int height){ m_height = height;};
    
    int getWidth(int width){return m_width;};
    int getHeight(int height){return m_height;};
};

#endif /* NEScreenRenderer_hpp */
