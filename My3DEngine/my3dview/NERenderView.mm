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
#import "NEDepthBuffer.hpp"

@interface NERenderView()

@property (nonatomic) NSArray<NSValue*>* vectices;

@property (nonatomic) NECamera* camera;

@property (nonatomic) NEPolygonLine* xAxis;
@property (nonatomic) NEPolygonLine* yAxis;
@property (nonatomic) NEPolygonLine* zAxis;

@property (nonatomic) NEDepthBuffer depthBuffer;

//@property (nonatomic) NEVector2 lineBuffer[];

@end

#define MAX_LINE_BUF 2048

#define COORD_AMPLIFY_FACTOR 3

@implementation NERenderView{
    NEVector3 _lineBuffer[MAX_LINE_BUF];
}

- (id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        [self doInit];
    }
    return self;
}

- (void)doInit{
    NECamera * camera = [[NECamera alloc] init];
//    camera.position = GLKVector3Make(2, 2, 2);
//    [camera lookAt:GLKVector3Make(-1, -1, -2)];
//    camera.yAxis = GLKVector3Make(-1, -1, 1);
    
    camera.position = GLKVector3Make(20, 20, 15);
    NEVector3 pointToLookAt = GLKVector3Make(0, 0, 5);
    [camera lookAt:GLKVector3Make(pointToLookAt.x - camera.position.x, pointToLookAt.y - camera.position.y, pointToLookAt.z - camera.position.z)];
//    [camera lookAt:GLKVector3Make(-5, -1, -3)];
    
//    float yAxis_x = -0.7, yAxis_y = 1;
//    camera.yAxis = getVerticalVec(camera.lookAtDirection, &yAxis_x, &yAxis_y, NULL);
////    camera.yAxis = GLKVector3Make(0, 0, 2);
    //find y axis which together with z form a plane vertical to xy plane
    NEVector3 lookAtRotated_90= { - camera.lookAtDirection.y , camera.lookAtDirection.x, 0};
    NEVector3 cam_yAxis = crossVectors(camera.lookAtDirection, lookAtRotated_90);
    camera.yAxis = cam_yAxis;
    
    self.camera = camera;
    
    NEPolygonLine * xAxis = [[NEPolygonLine alloc] init];
    xAxis.startPosition = GLKVector3Make(0, 0, 0);
    xAxis.endPosition = GLKVector3Make(5, 0, 0);
   
    NEPolygonLine * yAxis = [[NEPolygonLine alloc] init];
    yAxis.startPosition = GLKVector3Make(0, 0, 0);
    yAxis.endPosition = GLKVector3Make(0, 5, 0);
   
    NEPolygonLine * zAxis = [[NEPolygonLine alloc] init];
    zAxis.startPosition = GLKVector3Make(0, 0, 0);
    zAxis.endPosition = GLKVector3Make(0, 0, 5);
    
    self.xAxis = xAxis;
    self.yAxis = yAxis;
    self.zAxis = zAxis;
    
    _depthBuffer.resetSize(self.frame.size.width, self.frame.size.height);
}

- (CGPoint)positionInView:(NEVector3)originalPoint{
    return [self positionInView:originalPoint projectResultNDC:nil];
}

- (CGPoint)positionInView:(NEVector3)originalPoint projectResultNDC:(NEVector3 *)pointTran{
    CGFloat width = self.frame.size.width * COORD_AMPLIFY_FACTOR;
    CGFloat height = self.frame.size.height * COORD_AMPLIFY_FACTOR;
    
    NEVector3 point = getPositionInCameraCoordinateSystem(originalPoint, self.camera.position, self.camera.lookAtDirection, self.camera.yAxis);
    
    NEVector3 point_tran = perspetiveProjectPoint(point, self.camera.frustum);
    
    if (pointTran) {
        *pointTran = point_tran;
    }
    
    CGFloat screen_x = (1 +  point_tran.x)* width/2;
    CGFloat screen_y = ( 1 -  point_tran.y)* height/2;
    
    return CGPointMake(screen_x, screen_y);
}

- (CGPoint)pointInVewForVector3:(NEVector3)vector{
    CGFloat width = self.frame.size.width * COORD_AMPLIFY_FACTOR;
    CGFloat height = self.frame.size.height * COORD_AMPLIFY_FACTOR;
    
    CGFloat screen_x = (1 +  vector.x)* width/2;
    CGFloat screen_y = ( 1 -  vector.y)* height/2;
    
    return CGPointMake(screen_x, screen_y);
}

- (void)drawOrigin{
    CGPoint p0 = [self positionInView:GLKVector3Make(0, 0, 0)];
    
    CGContextRef context = UIGraphicsGetCurrentContext();

    // Draw them with a 2.0 stroke width so they are a bit more visible.
    CGContextSetFillColorWithColor(context, UIColor.whiteColor.CGColor);

    CGContextAddEllipseInRect(context, CGRectMake(p0.x - 5, p0.y - 5, 10, 10));
    // and now draw the Path!
    CGContextFillPath(context);
}

- (void)drawLine:(NEPolygonLine *)line color:(UIColor *)color {
    [self drawPointsForLine:line color:color];
    return;
    
//    CGPoint p0 = [self positionInView:line.startPosition];
//    CGPoint p1 = [self positionInView:line.endPosition];
//
//    CGContextRef context = UIGraphicsGetCurrentContext();
//    CGContextSetStrokeColorWithColor(context, [UIColor redColor].CGColor);
//
//    // Draw them with a 2.0 stroke width so they are a bit more visible.
//    CGContextSetLineWidth(context, 2.0f);
//
//    CGContextSetStrokeColorWithColor(context, color.CGColor);
//
//    CGContextMoveToPoint(context, p0.x, p0.y); //start at this point
//
//    CGContextAddLineToPoint(context, p1.x, p1.y); //draw to this point
//
//    // and now draw the Path!
//    CGContextStrokePath(context);
}


- (void)drawPointsForLine:(NEPolygonLine *)line color:(UIColor *)color {
    NEVector3 p0Tran, p1Tran;
    
    [self positionInView:line.startPosition projectResultNDC:&p0Tran];
    [self positionInView:line.endPosition projectResultNDC:&p1Tran];
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, color.CGColor);

    // Draw them with a 2.0 stroke width so they are a bit more visible.
    int bufSize;
    getPointsArrayInLine(p0Tran, p1Tran, _lineBuffer, MAX_LINE_BUF, &bufSize);
    
    for (int i = 0; i < bufSize; i++) {
        NEVector3 point = _lineBuffer[i];
        CGPoint pointInVew = [self pointInVewForVector3:point];
        float oldZ = _depthBuffer.getZ((int)pointInVew.x, (int)pointInVew.y);
        if (point.z > oldZ) {
            
            CGContextFillRect(context, CGRectMake(pointInVew.x / COORD_AMPLIFY_FACTOR, pointInVew.y / COORD_AMPLIFY_FACTOR, 1, 1));
            _depthBuffer.setZ(point.z, (int)pointInVew.x, (int)pointInVew.y);
        }
    }

}

- (void)drawRect:(CGRect)rect{
    [self drawOrigin];
    
    [self drawLine:self.xAxis color:[UIColor redColor]];
    [self drawLine:self.yAxis color:[UIColor greenColor]];
    [self drawLine:self.zAxis color:[UIColor blueColor]];
}

@end
