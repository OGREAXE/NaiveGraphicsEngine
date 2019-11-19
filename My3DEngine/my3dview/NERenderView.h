//
//  MERenderView.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class NEPolygonLine;

@interface NERenderView : UIView

@property (nonatomic) float moveSpeed;

@property (nonatomic) float rotationRate;

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

@end

NS_ASSUME_NONNULL_END
