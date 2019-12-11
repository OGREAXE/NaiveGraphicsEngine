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

typedef struct tagTextureParam {
    NEVector2 uv0;
    NEVector2 uv1;
    NEVector2 uv2;
    
    NEVector4 equation_coeffient_u;
    NEVector4 equation_coeffient_v;
    
    bool hasTexture;
    int textureIndex;
} TextureParam;

typedef struct tagDrawParam {
    //params pass down
    //camera space
    NEVector3 vert0c;
    NEVector3 vert1c;
    NEVector3 vert2c;
    
    NEVector3 normal0c;
    NEVector3 normal1c;
    NEVector3 normal2c;
    
    ////ndc space
    NEVector3 position_t;
    
    NEVector3 vert0t;
    NEVector3 vert1t;
    NEVector3 vert2t;
    
    TextureParam textureParam;
    
    //params pass back up
    float intensity0;
    float intensity1;
    float intensity2;
    
} DrawParam;

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
    
    void createRenderBuffer(int size);
    
protected:
    //overwrite this method to do color blending/lighting
    //all parameters are in camera space
    virtual float colorBlendResult(float color, NEVector3 &position, DrawParam &param,  void *extraInfo){return color;}
    
    //overwrite this method to prepare for drawing
    virtual void prepareDrawMeshes(const std::vector<NEMesh> &meshes){};
    
    virtual void finishDrawMeshes(const std::vector<NEMesh> &meshes){};
    
    virtual void prepareDrawFace(const NEFace &face, DrawParam &param){};
public:
    //public members
    NECamera camera;
public:
    NEStandardRenderer(int width, int height);
    
    virtual ~NEStandardRenderer(){
        free(_renderBuffer);
    }
    
//    void setScreenSize(int width, int height){
//        _screenWidth = width;
//        _screenHeight = height;
//    }
    
    //called after setScreenSize
    void createDefaultRenderBuffer();
    
    NEDepthBuffer &depthBuffer(){return _depthBuffer;}
    
    void drawMeshes(const std::vector<NEMesh> &meshes);
    
    const RenderBufferType * getRenderBuffer(int &size){ size = _renderBufferSize; return _renderBuffer;}
};

#endif /* NEStandardRenderer_hpp */
