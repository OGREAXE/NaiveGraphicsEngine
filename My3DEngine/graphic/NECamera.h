//
//  MECamera.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NECommon.h"

NS_ASSUME_NONNULL_BEGIN

@interface NECamera : NSObject

@property (nonatomic) NEVector3 position;

@property (nonatomic) NEVector3 lookAtPosition; //the direction the Z axis pointing in world coordinate system

- (void)lookAt:(NEVector3)position;

@end

NS_ASSUME_NONNULL_END
