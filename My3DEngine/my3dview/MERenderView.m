//
//  MERenderView.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "MERenderView.h"
#import "MECommon.h"
#import "MEPolygonLine.h"
#import <math.h>

MEVector3 getPositionInCameraCoordinateSystem(MEVector3 worldPosition, MEVector3 cameraPositionInWorld, MEVector3 cameraLookAt, MEVector3 cameraYAxisInWorld){
    
}

@interface MERenderView()

@property (nonatomic) NSArray<NSValue*>* vectices;

@property (nonatomic) MECamera* camera;

@property (nonatomic) MEPolygonLine* xAxis;
@property (nonatomic) MEPolygonLine* yAxis;
@property (nonatomic) MEPolygonLine* zAxis;

@end

@implementation MERenderView

- (id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        [self doInit];
    }
    return self;
}

- (void)doInit{
    MECamera * camera = [[MECamera alloc] init];
    camera.position = GLKVector3Make(1, 1, 1);
    [camera lookAt:GLKVector3Make(0, 0, 0)];
    self.camera = camera;
    
    MEPolygonLine * xAxis = [[MEPolygonLine alloc] init];
    xAxis.startPosition = GLKVector3Make(0, 0, 0);
    xAxis.endPosition = GLKVector3Make(1, 0, 0);
   
    MEPolygonLine * yAxis = [[MEPolygonLine alloc] init];
    yAxis.startPosition = GLKVector3Make(0, 0, 0);
    yAxis.endPosition = GLKVector3Make(0, 1, 0);
   
    MEPolygonLine * zAxis = [[MEPolygonLine alloc] init];
    zAxis.startPosition = GLKVector3Make(0, 0, 0);
    zAxis.endPosition = GLKVector3Make(0, 0, 1);
    
    self.xAxis = xAxis;
    self.yAxis = yAxis;
    self.zAxis = zAxis;
}

- (void)drawRect:(CGRect)rect{
    
}

@end
