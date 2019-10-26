//
//  MELine.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "MEObject.h"
#import "MECommon.h"

NS_ASSUME_NONNULL_BEGIN

@interface MELine : MEObject

- (id)initWithStart:(MEVector3)start end:(MEVector3)end;

@end

NS_ASSUME_NONNULL_END
