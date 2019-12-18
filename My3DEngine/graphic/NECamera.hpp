//
//  NECamera.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/12/5.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NECamera_hpp
#define NECamera_hpp

#include <stdio.h>
#include "NEMath.h"

class NECamera {
private:
    bool _isOthorgraphics = false;
public:
    NEVector3 position; //position in world coordinate
    NEVector3 lookAtDirection;
    NEVector3 yAxis; //y axis vector, world coordinate
    NEVector3 xAxis; //y axis vector, world coordinate
    NEFrustum frustum;
    
    NEVector3 worldIOriginInCameraCoord;
    NEVector3 worldXAxisInCameraCoord;
    NEVector3 worldYAxisInCameraCoord;
    NEVector3 worldZAxisInCameraCoord;
    
    //world to camera matrixes
    NEMatrix3 rotation_w2c_z_axis_mat;
    NEMatrix3 rotation_w2c_y_axis_mat;
    
    //camera to world matrixes
    NEMatrix3 rotation_c2w_z_axis_mat;
    NEMatrix3 rotation_c2w_y_axis_mat;
    
    NECamera();
    NECamera(NEFrustum frustum):frustum(frustum){}
    
    void setWindow(float width, float height);
    
    void setOthorgraphics(bool othor) {_isOthorgraphics = othor;}
    bool isOthorgraphics(){return _isOthorgraphics;}
    
    void lookAtPoint(NEVector3 direction);

    void normalize();
    void rotateByNearHorizontallyByDegree(float degree);
    void rotateByNearVerticallyByDegree(float degree);
    
    void updateWorldAxis(){
        worldIOriginInCameraCoord = getPositionInCameraCoordinateSystem(NEVector3Make(0, 0, 0), position, lookAtDirection, yAxis);
        
        worldXAxisInCameraCoord = getPositionInCameraCoordinateSystem(NEVector3Make(1, 0, 0), position, lookAtDirection, yAxis);
        
        worldXAxisInCameraCoord = NEVector3Subtract(worldXAxisInCameraCoord, worldIOriginInCameraCoord);
        
        worldYAxisInCameraCoord = getPositionInCameraCoordinateSystem(NEVector3Make(0, 1, 0), position, lookAtDirection, yAxis);
        
        worldYAxisInCameraCoord = NEVector3Subtract(worldYAxisInCameraCoord, worldIOriginInCameraCoord);
        
        worldZAxisInCameraCoord = getPositionInCameraCoordinateSystem(NEVector3Make(0, 0, 1), position, lookAtDirection, yAxis);
        
        worldZAxisInCameraCoord = NEVector3Subtract(worldZAxisInCameraCoord, worldIOriginInCameraCoord);
        
        NEGetCoordConvertionRotationMatrixes(position, lookAtDirection, yAxis, &rotation_w2c_z_axis_mat, &rotation_w2c_y_axis_mat);
        
        NEGetCoordConvertionRotationMatrixes(worldIOriginInCameraCoord, worldZAxisInCameraCoord, worldYAxisInCameraCoord, &rotation_c2w_z_axis_mat, &rotation_c2w_y_axis_mat);
    }
    
    inline NEVector3 getWorldPosition(NEVector3 &positionInCameraSpace){
//        return convertPositionFromOriginalCoordSystem(positionInCameraSpace, worldIOriginInCameraCoord, worldZAxisInCameraCoord, worldYAxisInCameraCoord);
        
        return NEPostionConvertFromOriginalCoordSystem(positionInCameraSpace, worldIOriginInCameraCoord, &rotation_c2w_z_axis_mat, &rotation_c2w_y_axis_mat);

    }
    
    inline NEVector3 positionInCameraCoordinateSystem(NEVector3 &worldPosition){
        NEVector3 pos = NEPostionConvertFromOriginalCoordSystem(worldPosition, position, &rotation_w2c_z_axis_mat, &rotation_w2c_y_axis_mat);
        
        //rotate it by 180 degrees around y axis to turn into eye space (in accordance with opengl frustum)
        NEVector3 ret = { - pos.x, pos.y, - pos.z};
        return ret;
    }
};

#endif /* NECamera_hpp */
