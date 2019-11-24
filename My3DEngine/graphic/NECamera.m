//
//  MECamera.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "NECamera.h"

@implementation NECamera

- (id)init{
    self = [super init];
    if (self) {
        NEFrustum fru;
        fru.r = 0.2;
        fru.t = 0.2;
        
        fru.near = 1;
        fru.far = 100;
        
        self.frustum = fru;
    }
    return self;
}

- (void)lookAt:(NEVector3)direction{
    self.lookAtDirection = direction;
}

- (void)normalize{
    _lookAtDirection = getNormalizedVector(_lookAtDirection);
    _xAxis = getNormalizedVector(_xAxis);
    _yAxis = getNormalizedVector(_yAxis);
}

#define NEAR_FACTOR 1
- (void)rotateByNearHorizontallyByDegree:(float)degree{
    [self normalize];
    
    float nearDistance = _frustum.near * NEAR_FACTOR;
    NEVector3 pointInRotatedAxis = NEVector3Make(self.position.x + nearDistance * self.lookAtDirection.x, self.position.y + nearDistance * self.lookAtDirection.y, self.position.z + nearDistance * self.lookAtDirection.z);
    
    self.xAxis = rotationByAngle(self.xAxis, self.yAxis, degree);
    self.lookAtDirection = rotationByAngle(self.lookAtDirection, self.yAxis, degree);
    
    self.position = rotationByAngleAroundLine2(self.position, self.yAxis, pointInRotatedAxis, degree);
}

- (void)rotateByNearVerticallyByDegree:(float)degree{
    [self normalize];
    
    float nearDistance = _frustum.near * NEAR_FACTOR;
    NEVector3 pointInRotatedAxis = NEVector3Make(self.position.x + nearDistance * self.lookAtDirection.x, self.position.y + nearDistance * self.lookAtDirection.y, self.position.z + nearDistance * self.lookAtDirection.z);
    
    self.yAxis = rotationByAngle(self.yAxis, self.xAxis, degree);
    self.lookAtDirection = rotationByAngle(self.lookAtDirection, self.xAxis, degree);
    
    self.position = rotationByAngleAroundLine2(self.position, self.xAxis, pointInRotatedAxis, degree);
}

@end
