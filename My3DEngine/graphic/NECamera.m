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
        fru.far = 10;
        
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

@end
