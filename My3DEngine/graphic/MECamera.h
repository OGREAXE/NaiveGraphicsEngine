//
//  MECamera.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MECommon.h"

NS_ASSUME_NONNULL_BEGIN

@interface MECamera : NSObject

@property (nonatomic) MEVector3 position;

@property (nonatomic) MEVector3 lookAtPosition; //the direction the Z axis pointing in world coordinate system

- (void)lookAt:(MEVector3)position;

@end

NS_ASSUME_NONNULL_END
