//
//  NELine.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "NEObject.h"
#import "NECommon.h"

NS_ASSUME_NONNULL_BEGIN

@interface NELine : NEObject

- (id)initWithStart:(NEVector3)start end:(NEVector3)end;

@end

NS_ASSUME_NONNULL_END
