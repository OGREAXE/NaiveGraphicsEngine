//
//  NEDotLight.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/6.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEDotLight_hpp
#define NEDotLight_hpp

#include <stdio.h>
#include "NEDotLightRenderer.hpp"
#include "NEMath.h"

class NEDotLight {
    NEVector3 _position;
    
    NEDotLightRenderer *_renderer;
    
    bool _needRedrawMap;
    
    NEVector3 _positionInRealCameraSpace;
    NEVector3 _zAxisInRealCameraSpace;
    NEVector3 _yAxisInRealCameraSpace;
public:
    NEDotLight(){
        _renderer = new NEDotLightRenderer(1000, 1000);
        _needRedrawMap = true;
    }
    ~NEDotLight(){
        delete _renderer;
    }
    
    NEVector3 &position(){return _position;}
    void setPosition(NEVector3 &position){
        _position = position;
        _renderer->camera.position = position;
    }
    
    void setRenderDestination(NEVector3 destinationPoint){
        _renderer->camera.lookAtPoint(destinationPoint);
    }
    
    NEVector3 getDirection_world(){
        return _renderer->camera.lookAtDirection;
    }
    
    void prepareRander(){
        _renderer->camera.updateWorldAxis();
    }
    
    void renderIfNeed(const std::vector<NEMesh> &meshes);
    
    void setNeedUpdate(){
        _needRedrawMap = true;
    }
    
    bool canTouchPosition(NEVector3 &worldPos){
        int res =_renderer->testPosition_world(worldPos);
        
        return res == 1;
    }
    
    void updateGeometricsByRealCamera(NECamera &realCamera){
        NECamera &lightCam = _renderer->camera;
        
        _positionInRealCameraSpace = convertPositionFromOriginalCoordSystem(lightCam.position, realCamera.position, realCamera.lookAtDirection, realCamera.yAxis);
        
        _zAxisInRealCameraSpace = NEConvertVectorToCoordSystem( lightCam.lookAtDirection, realCamera.position, realCamera.lookAtDirection, realCamera.yAxis);
        
        _yAxisInRealCameraSpace = NEConvertVectorToCoordSystem( lightCam.yAxis, realCamera.position, realCamera.lookAtDirection, realCamera.yAxis);
        
    }
    
    NEVector3 convertPostionInRealCameraSpaceToLightSpace(NEVector3 &positionInRealCamSpace){
        return convertPositionFromOriginalCoordSystem(positionInRealCamSpace, _positionInRealCameraSpace, _zAxisInRealCameraSpace, _yAxisInRealCameraSpace);
    }
};

#endif /* NEDotLight_hpp */
