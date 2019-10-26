//
//  MEObject.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MEPolygon.h"

NS_ASSUME_NONNULL_BEGIN

@interface MEObject : NSObject

@property (nonatomic) MEPolygon * polygon;

@end

NS_ASSUME_NONNULL_END
