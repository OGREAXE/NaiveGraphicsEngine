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

@property (nonatomic) NEVector3 position; //position in world coordinate

@property (nonatomic) NEVector3 lookAtDirection; //actually is the Z axis vector, world coordinate

@property (nonatomic) NEVector3 yAxis; //y axis vector, world coordinate

- (void)lookAt:(NEVector3)direction;

@end

NS_ASSUME_NONNULL_END
