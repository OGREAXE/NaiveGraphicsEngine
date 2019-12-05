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
    
    
public:
    NEVector3 position; //position in world coordinate
    NEVector3 lookAtDirection;
    NEVector3 yAxis; //y axis vector, world coordinate
    NEVector3 xAxis; //y axis vector, world coordinate
    NEFrustum frustum;
    
    NECamera();
    NECamera(NEFrustum frustum):frustum(frustum){}
    
    void lookAtPoint(NEVector3 direction);

    void normalize();
    void rotateByNearHorizontallyByDegree(float degree);
    void rotateByNearVerticallyByDegree(float degree);
};

#endif /* NECamera_hpp */
