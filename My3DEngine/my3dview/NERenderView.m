//
//  MERenderView.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "NERenderView.h"
#import "NECommon.h"
#import "NEPolygonLine.h"

@interface NERenderView()

@property (nonatomic) NSArray<NSValue*>* vectices;

@property (nonatomic) NECamera* camera;

@property (nonatomic) NEPolygonLine* xAxis;
@property (nonatomic) NEPolygonLine* yAxis;
@property (nonatomic) NEPolygonLine* zAxis;

@end

@implementation NERenderView

- (id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        [self doInit];
    }
    return self;
}

- (void)doInit{
    NECamera * camera = [[NECamera alloc] init];
    camera.position = GLKVector3Make(1, 1, 1);
    [camera lookAt:GLKVector3Make(0, 0, 0)];
    self.camera = camera;
    
    NEPolygonLine * xAxis = [[NEPolygonLine alloc] init];
    xAxis.startPosition = GLKVector3Make(0, 0, 0);
    xAxis.endPosition = GLKVector3Make(1, 0, 0);
   
    NEPolygonLine * yAxis = [[NEPolygonLine alloc] init];
    yAxis.startPosition = GLKVector3Make(0, 0, 0);
    yAxis.endPosition = GLKVector3Make(0, 1, 0);
   
    NEPolygonLine * zAxis = [[NEPolygonLine alloc] init];
    zAxis.startPosition = GLKVector3Make(0, 0, 0);
    zAxis.endPosition = GLKVector3Make(0, 0, 1);
    
    self.xAxis = xAxis;
    self.yAxis = yAxis;
    self.zAxis = zAxis;
}

- (void)drawRect:(CGRect)rect{
    
}

@end
