//
//  NEPolygonLine.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NECommon.h"
#import "NEPolygon.h"

NS_ASSUME_NONNULL_BEGIN

@interface NEPolygonLine : NEPolygon

@property (nonatomic) NEVector3 startPosition;

@property (nonatomic) NEVector3 endPosition;

@end

NS_ASSUME_NONNULL_END
