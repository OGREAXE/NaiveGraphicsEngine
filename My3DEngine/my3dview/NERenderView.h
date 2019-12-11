//
//  MERenderView.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <UIKit/UIKit.h>
#include "NEMesh.h"
#include <vector>

NS_ASSUME_NONNULL_BEGIN

@class NEPolygonLine;

@protocol NERenderView : UIView

@end

@interface NERenderView : UIView

@property (nonatomic) float moveSpeed;

@property (nonatomic) float rotationRate;

@property (nonatomic) BOOL lineFrameMode;

@property (nonatomic) NSArray<NEPolygonLine*> *frameLines;

- (void)moveForward;
- (void)moveBack;

- (void)moveUp;
- (void)moveDown;
- (void)moveLeft;
- (void)moveRight;

- (void)lookUp;
- (void)lookDown;
- (void)turnLeft;
- (void)turnRight;

- (void)loadMeshes:(std::vector<NEMesh> &)meshes;

@end

NS_ASSUME_NONNULL_END
