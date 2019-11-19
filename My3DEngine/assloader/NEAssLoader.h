//
//  NEAssLoader.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/19.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NEMath.h"

NS_ASSUME_NONNULL_BEGIN

@class NEAssLoader;

@protocol NEAssLoaderDelegate <NSObject>

@end

@class NEPolygonLine;

@interface NEAssLoader : NSObject

@property (nonatomic, weak) id<NEAssLoaderDelegate> delegate;

- (NSArray<NEPolygonLine*>*)lineFrameFromMeshAt:(int)meshIndex range:(float)range;

@end

NS_ASSUME_NONNULL_END
