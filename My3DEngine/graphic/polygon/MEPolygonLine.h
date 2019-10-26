//
//  MEPolygonLine.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MECommon.h"
#import "MEPolygon.h"

NS_ASSUME_NONNULL_BEGIN

@interface MEPolygonLine : MEPolygon

@property (nonatomic) MEVector3 startPosition;

@property (nonatomic) MEVector3 endPosition;

@end

NS_ASSUME_NONNULL_END
