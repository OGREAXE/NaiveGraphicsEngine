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
        fru.r = 0.5;
        fru.t = 0.5;
        
        fru.near = 0.5;
        fru.far = 100;
        
        self.frustum = fru;
    }
    return self;
}

- (void)lookAt:(NEVector3)direction{
    self.lookAtDirection = direction;
}

@end
