//
//  NEComposedRenderer.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEComposedRenderer_hpp
#define NEComposedRenderer_hpp

#include <stdio.h>
#include "NEStandardRenderer.hpp"
#include <vector>
#include "NEDotLight.hpp"
#include "NECommon.h"
#include "NEDisplayDevice.hpp"
#include "NETextureProvider.hpp"

class NEComposedRenderer:public NEStandardRenderer{
    NEDotLight *_dotLight0;
    
    NEVector3 _dotLightPositionInCameraSpace;
    
    NEDisplayDevice *_device;
    
    NETextureProvider *_textureProvider;
    
    std::vector<NEMaterial> _materials;
    
    bool _trueShadow;
    
protected:
    NEComposedRenderer();
    
    virtual void prepareDrawMeshes(const std::vector<NEMesh> &meshes);
    
    virtual void finishDrawMeshes(const std::vector<NEMesh> &meshes);
    
    virtual float FragmentShaderFunc(float color, NEVector3 &position,  ShaderParam &param, void *extraInfo);
    
    virtual void prepareDrawFace(const NEFace &face, ShaderParam &param);
public:
    NEComposedRenderer(int width, int height):NEStandardRenderer(width, height){
//        NEStandardRenderer(width, height);
        createDefaultRenderBuffer();
        _trueShadow = true;
        
        _textureProvider = nullptr;
    }
    
    void setTrueShadow(bool trueShadow){ _trueShadow = trueShadow;}
    
    ~NEComposedRenderer(){
        NESafeDelete(_dotLight0);
    }
    
    void setDevice(NEDisplayDevice *device){_device = device;}
    
    void setTextureProvider(NETextureProvider *provider){ _textureProvider = provider; }
    
    void setMaterials(std::vector<NEMaterial> &materials){ _materials = materials; }
    
    void createDotLightAt(NEVector3 position, NEVector3 renderDestination){
        this->_dotLight0 = new NEDotLight();
        _dotLight0->setPosition(position);
        _dotLight0->setRenderDestination(renderDestination);
    }
};

#endif /* NEComposedRenderer_hpp */
