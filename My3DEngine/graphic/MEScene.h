//
//  MEScene.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MECommon.h"
#import "MECamera.h"
#import "MEPolygon.h"

NS_ASSUME_NONNULL_BEGIN

@interface MEScene : NSObject

@property (nonatomic) MECamera * camera;

@property (nonatomic) NSArray<MEObject* >* objects;

@end

NS_ASSUME_NONNULL_END
