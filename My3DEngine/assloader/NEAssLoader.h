//
//  NEAssLoader.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/19.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NEMath.h"
#include "NEMesh.h"
#include "CNEAssReader.hpp"

typedef struct tagLoadParam {
    NEVector3 position;
    NEVector3 rotation;
    float width;
} NELoadAssParam;

NS_ASSUME_NONNULL_BEGIN

@class NEAssLoader;

@protocol NEAssLoaderDelegate <NSObject>

- (void)loader:(NEAssLoader *)loader didLoadMeshes:(const std::vector<NEMesh> &)meshes;

@end

@class NEPolygonLine;

@interface NEAssLoader : NSObject

@property (nonatomic, weak) id<NEAssLoaderDelegate> delegate;

- (NSArray<NEPolygonLine*>*)lineFrameFromMeshAt:(int)meshIndex range:(float)range;

- (NSArray<NEPolygonLine*>*)lineFrameFromMesh:(NEMesh &)mesh range:(float)range;

- (void)loadFbx:(NSString *)modelName param:(NELoadAssParam)param;

- (void)loadDefaultFile;

- (void)upload;

- (void)reset;

@end

NS_ASSUME_NONNULL_END
