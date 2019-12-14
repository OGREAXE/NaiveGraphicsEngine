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

enum NETextureType
{
    NETextureType_NONE = 0,
    NETextureType_DIFFUSE = 1,
    NETextureType_SPECULAR = 2,
};

typedef struct tagTextureParam {
    NETextureType type;
    int index;
    std::string name;
} NETexureParam;

typedef struct tagMaterialParam {
    NEVector2 uv0;
    NEVector2 uv1;
    NEVector2 uv2;
    
    NEVector2 uv0_z; // = uv0/z
    NEVector2 uv1_z;
    NEVector2 uv2_z;
    
    float interpl_uz_start;
    float interpl_uz_end;
    float interpl_uz_diff;
    
    float interpl_vz_start;
    float interpl_vz_end;
    float interpl_vz_diff;
    
    bool hasTexture;
    int materialIndex;
    
    std::vector<NETexureParam> textureStack;
} NEMaterialParam;

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
    
    NEMaterialParam material;
    
    //params pass back up
    float intensity0;
    float intensity1;
    float intensity2;
    
    //interpolated value
    //when newline is set to true, need to cal the following interpolate arguments
    bool newLine = true;
    
    NEVector2 interpolate_jointLeft;
    NEVector2 interpolate_jointRight;
    
//    float interpl_start;
//    float interpl_end;
    
    float interpl_intensity_start;
    float interpl_intensity_end;
    float interpl_intensity_diff;
    
    float interpl_divide_factor;
} ShaderParam;

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
    NEVector3 convertToCameraSpace(NEVector3 &originalPoint);
    NEVector3 convertToEyeSpace(NEVector3 &originalPoint);
    NEVector3 convertVectorToCameraSpace(NEVector3 &aVector, NEVector3 &originInCameraSpace);
    NEVector3 convertVectorToEyeSpace(NEVector3 &aVector, NEVector3 &originInEyeSpace);
    
    NEVector2 pointInVewForVector3(NEVector3 vector);
    
    void createRenderBuffer(int size);
    
protected:
    //overwrite this method to do color blending/lighting
    //all parameters are in camera space
    virtual float FragmentShaderFunc(float color, NEVector3 &position, ShaderParam &param,  void *extraInfo){return color;}
    
    //overwrite this method to prepare for drawing
    virtual void prepareDrawMeshes(const std::vector<NEMesh> &meshes){};
    
    virtual void finishDrawMeshes(const std::vector<NEMesh> &meshes){};
    
    virtual void prepareDrawFace(const NEFace &face, ShaderParam &param){};
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
