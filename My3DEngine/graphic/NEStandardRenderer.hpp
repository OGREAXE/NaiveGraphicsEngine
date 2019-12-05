//
//  NEStandardRenderer.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEStandardRenderer_hpp
#define NEStandardRenderer_hpp

#include <stdio.h>
#include "NEDepthBuffer.hpp"
#include "NECamera.hpp"
#include "NEMesh.h"

typedef long long RenderBufferType;

class NEStandardRenderer {
    NEDepthBuffer _depthBuffer;
    
    RenderBufferType *_renderBuffer;
    
    int _renderBufferSize;
    
    NECamera _camera;
    
    int _screenWidth;
    
    int _screenHeight;
    
    NEVector3 convertToCameraSpace(NEVector3 originalPoint);
    NEVector3 convertToEyeSpace(NEVector3 originalPoint);
    NEVector3 convertVectorToCameraSpace(NEVector3 &aVector, NEVector3 &originInCameraSpace);
    NEVector3 convertVectorToEyeSpace(NEVector3 &aVector, NEVector3 &originInEyeSpace);
    
    NEVector2 pointInVewForVector3(NEVector3 vector);
    
    void drawMesh(const NEMesh &mesh);
    
protected:
    
public:
    NEDepthBuffer &depthBuffer(){return _depthBuffer;}
    
    void drawMeshes(const std::vector<NEMesh> &meshes);
    
    const RenderBufferType * getRenderBuffer(int &size){ size = _renderBufferSize; return _renderBuffer;}
};

#endif /* NEStandardRenderer_hpp */
