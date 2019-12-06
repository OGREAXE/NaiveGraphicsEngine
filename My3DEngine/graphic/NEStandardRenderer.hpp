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
#include "NECommon.h"

typedef long long RenderBufferType;

class NEStandardRenderer {
private:
    
protected:
    NEStandardRenderer(){}
    
    NEDepthBuffer _depthBuffer;
    
    RenderBufferType *_renderBuffer;
    
    int _renderBufferSize;
    
    int _screenWidth;
    
    int _screenHeight;
    
    void drawMesh(const NEMesh &mesh);

protected:
    //util methods
    NEVector3 convertToCameraSpace(NEVector3 originalPoint);
    NEVector3 convertToEyeSpace(NEVector3 originalPoint);
    NEVector3 convertVectorToCameraSpace(NEVector3 &aVector, NEVector3 &originInCameraSpace);
    NEVector3 convertVectorToEyeSpace(NEVector3 &aVector, NEVector3 &originInEyeSpace);
    
    NEVector2 pointInVewForVector3(NEVector3 vector);
    
    void createRenderBuffer(int size){
        if (size > 0) {
            _renderBuffer =
            (RenderBufferType *)
            malloc(sizeof(RenderBufferType) * size);
        }
    }
    
protected:
    //overwrite this method to do color blending/lighting
    //all parameters are in camera space
    virtual float colorBlendResult(float color, NEVector3 &position, NEVector3 &normal, void *extraInfo){return color;}
    
    //overwrite this method to prepare for drawing
    virtual void prepareDrawMeshes(const std::vector<NEMesh> &meshes){};
    
    virtual void finishDrawMeshes(const std::vector<NEMesh> &meshes){};
public:
    //public members
    NECamera camera;
public:
    NEStandardRenderer(int width, int height):_screenWidth(width), _screenHeight(height){
        _renderBuffer = nullptr;
        _depthBuffer.resetSize(width * COORD_AMPLIFY_FACTOR, height * COORD_AMPLIFY_FACTOR);
    }
    
    virtual ~NEStandardRenderer(){
        free(_renderBuffer);
    }
    
//    void setScreenSize(int width, int height){
//        _screenWidth = width;
//        _screenHeight = height;
//    }
    
    //called after setScreenSize
    void createDefaultRenderBuffer(){
        createRenderBuffer(_screenHeight * _screenWidth);
        _renderBufferSize = 0;
    }
    
    NEDepthBuffer &depthBuffer(){return _depthBuffer;}
    
    void drawMeshes(const std::vector<NEMesh> &meshes);
    
    const RenderBufferType * getRenderBuffer(int &size){ size = _renderBufferSize; return _renderBuffer;}
};

#endif /* NEStandardRenderer_hpp */
