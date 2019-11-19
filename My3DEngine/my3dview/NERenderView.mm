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
#include <math.h>

#include "CNEAssReader.hpp"
#import "NEAssLoader.h"

// 创建颜色
#define RGBACOLOR(r,g,b,a)    ([UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:(a)])
#define RGBCOLOR(r,g,b)       RGBACOLOR(r,g,b,1)
#define HEXRGBACOLOR(h,a)     RGBACOLOR(((h>>16)&0xFF), ((h>>8)&0xFF), (h&0xFF), a)
#define HEXRGBCOLOR(h)        HEXRGBACOLOR(h,1)

@interface NERenderView()<NEAssLoaderDelegate>

@property (nonatomic) NSArray<NSValue*>* vectices;

@property (nonatomic) NECamera* camera;

@property (nonatomic) NEPolygonLine* xAxis;
@property (nonatomic) NEPolygonLine* yAxis;
@property (nonatomic) NEPolygonLine* zAxis;

//@property (nonatomic) NEPolygonLine* line0;

@property (nonatomic) NSArray<NEPolygonLine*> *geometries;

@property (nonatomic) NEDepthBuffer depthBuffer;

//@property (nonatomic) NEVector2 lineBuffer[];

@end

#define MAX_LINE_BUF 4096

#define COORD_AMPLIFY_FACTOR 1

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
    [self initCamera];
    
    [self initLineFrame];
    
    _depthBuffer.resetSize(self.frame.size.width * COORD_AMPLIFY_FACTOR, self.frame.size.height * COORD_AMPLIFY_FACTOR);
}

- (void)setFrameLines:(NSArray<NEPolygonLine *> *)frameLines{
    _frameLines = frameLines;
    [self initLineFrame];
}

- (void)initCamera{
    NECamera * camera = [[NECamera alloc] init];
        
    camera.position = GLKVector3Make(7, 7, 2);
    NEVector3 pointToLookAt = GLKVector3Make(0, 0, 2);
    
//    camera.position = GLKVector3Make(-0, 12, 0);
//    NEVector3 pointToLookAt = GLKVector3Make(0, 1, 0);
    
//    camera.position = GLKVector3Make(0, 10, 0);
//    NEVector3 pointToLookAt = GLKVector3Make(0, 12, 0);
    
    [camera lookAt:GLKVector3Make(pointToLookAt.x - camera.position.x, pointToLookAt.y - camera.position.y, pointToLookAt.z - camera.position.z)];
    //find y axis which together with z form a plane vertical to xy plane
    NEVector3 lookAtRotated_90= { - camera.lookAtDirection.y , camera.lookAtDirection.x, 0};
    NEVector3 cam_yAxis = crossVectors(camera.lookAtDirection, lookAtRotated_90);
    camera.yAxis = cam_yAxis;
    camera.xAxis = lookAtRotated_90;
    
    [camera normalize];
    
    self.camera = camera;
    
    self.moveSpeed = 0.2;
    
//    self.rotationRate = M_PI_2/100;
    
    self.rotationRate = M_PI_2 /16;
}

- (void)initLineFrame{
    [self initAxis];
    
    self.geometries = self.frameLines;
    
    /*
    NSMutableArray * geometries = [NSMutableArray array];
        
    //_line0 = [NEPolygonLine lineWithStart:GLKVector3Make(1, -2, 1) end:GLKVector3Make(1, 2, 1)];
    NSArray * points3 = @[
    @[@(1), @(1) ,@(1)], @[@(1), @(3) ,@(1)],
    @[@(1), @(3) ,@(1)], @[@(3), @(3) ,@(1)],
    @[@(3), @(3) ,@(1)],@[@(3), @(1) ,@(1)],
    @[@(3), @(1) ,@(1)],@[@(1), @(1) ,@(1)],
    
    @[@(1), @(1) ,@(3)], @[@(1), @(3) ,@(3)],
    @[@(1), @(3) ,@(3)], @[@(3), @(3) ,@(3)],
    @[@(3), @(3) ,@(3)],@[@(3), @(1) ,@(3)],
    @[@(3), @(1) ,@(3)],@[@(1), @(1) ,@(3)],
    
    @[@(1), @(1) ,@(1)], @[@(1), @(1) ,@(3)],
    @[@(1), @(3) ,@(1)], @[@(1), @(3) ,@(3)],
    @[@(3), @(3) ,@(1)],@[@(3), @(3) ,@(3)],
    @[@(3), @(1) ,@(1)],@[@(3), @(1) ,@(3)],
    
    ];
    
    for (int i = 0; i < points3.count/2; i++) {
        NSArray<NSNumber*> * endPoint = points3[2 * i];
        NSArray<NSNumber*> * startPoint = points3[2 * i + 1];
        
        NEPolygonLine * line = [NEPolygonLine lineWithStart:GLKVector3Make(startPoint[0].floatValue, startPoint[1].floatValue - 2, startPoint[2].floatValue -2 ) end:GLKVector3Make(endPoint[0].floatValue, endPoint[1].floatValue - 2, endPoint[2].floatValue - 2)];
        [geometries addObject:line];
    }
    
    self.geometries = geometries;
    
//    NEPolygonLine * line = [NEPolygonLine lineWithStart:GLKVector3Make(3, 1, 1) end:GLKVector3Make(1, 3, 1)];
//    [geometries addObject:line];
    */
}

- (void)initAxis{
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
}

- (CGPoint)positionInView:(NEVector3)originalPoint{
    return [self positionInView:originalPoint projectResultNDC:nil];
}

- (CGPoint)positionInView:(NEVector3)originalPoint projectResultNDC:(NEVector3 *)pointTran{
    CGFloat width = self.frame.size.width * COORD_AMPLIFY_FACTOR;
    CGFloat height = self.frame.size.height * COORD_AMPLIFY_FACTOR;
    
    NEVector3 point = getPositionInCameraCoordinateSystem(originalPoint, self.camera.position, self.camera.lookAtDirection, self.camera.yAxis);
    
    if (shoudTrimPoint(point, _camera.frustum)) {
        if (pointTran) {
            (*pointTran).x = 2;
            (*pointTran).y = 2;
        }
        
        return CGPointZero;
    }
    
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

- (void)drawLine:(NEPolygonLine *)line color:(long)color {
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


- (void)drawPointsForLine:(NEPolygonLine *)line color:(long)color {

    CGFloat fillWidth = 1./COORD_AMPLIFY_FACTOR;
    
    NEVector3 p0Tran, p1Tran;
    
    [self positionInView:line.startPosition projectResultNDC:&p0Tran];
    [self positionInView:line.endPosition projectResultNDC:&p1Tran];
    
    if (p0Tran.x < -1.5 || p0Tran.x > 1.5 || p1Tran.y < -1.5 || p1Tran.y > 1.5) {
        return;
    }
    
    CGContextRef context = UIGraphicsGetCurrentContext();

    
    int bufSize;
    
    getPointsArrayInLine(p0Tran, p1Tran, _lineBuffer, MAX_LINE_BUF, &bufSize);
    
    if (bufSize >= MAX_LINE_BUF) {
        return;
    }
    
    for (int i = 0; i < bufSize; i++) {
        NEVector3 point = _lineBuffer[i];
        CGPoint pointInVew = [self pointInVewForVector3:point];
        
        int posx = (int)pointInVew.x, posy = (int)pointInVew.y;
        
        if (posx > _depthBuffer.getWidth() || posx < 0 || posy > _depthBuffer.getHeight() || posy < 0) {
            continue;
        }
        
        DepthInfo info = _depthBuffer.getInfo(posx, posy);
        float oldZ = info.z;
        if (point.z <= oldZ) {
            CGContextSetFillColorWithColor(context, HEXRGBCOLOR(color).CGColor);
            CGContextFillRect(context, CGRectMake(posx / COORD_AMPLIFY_FACTOR - fillWidth/2, posy / COORD_AMPLIFY_FACTOR - fillWidth/2, fillWidth, fillWidth));
            info.z = point.z;
            info.color = color;
            _depthBuffer.setInfo(info, posx, posy);
        } else if(info.color != color){
//            NSLog(@"i");
//            CGContextSetFillColorWithColor(context, HEXRGBCOLOR(info.color).CGColor);
//            CGContextFillRect(context, CGRectMake((int)pointInVew.x / COORD_AMPLIFY_FACTOR - fillWidth/2, (int)pointInVew.y / COORD_AMPLIFY_FACTOR - fillWidth/2, fillWidth, fillWidth));
//            CGContextSetFillColorWithColor(context, HEXRGBCOLOR(color).CGColor);
        }
    }

}

- (void)drawRect:(CGRect)rect{
//    [self drawOrigin];
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextClearRect(context, self.bounds);
    
    _depthBuffer.resetSize();
    
    for (NEPolygonLine * line in _geometries) {
        [self drawLine:line color:0xffff00]; //yellow
    }
    
    [self drawLine:self.xAxis color:0xff0000];
    [self drawLine:self.yAxis color:0x00ff00];
    [self drawLine:self.zAxis color:0x0000ff];
}

- (void)redraw{
    NSLog(@"lookAtDirection %.5f, %.5f, %.5f", _camera.lookAtDirection.x, _camera.lookAtDirection.y, _camera.lookAtDirection.z);
    
    NSLog(@" y axis %.5f, %.5f, %.5f", _camera.yAxis.x, _camera.yAxis.y, _camera.yAxis.z);
    
    NSLog(@" x axis %.5f, %.5f, %.5f", _camera.xAxis.x, _camera.xAxis.y, _camera.xAxis.z);
    
    NSLog(@" position %.5f, %.5f, %.5f", _camera.position.x, _camera.position.y, _camera.position.z);
    
    NSLog(@"--------------------------");
    
//    NSLog(@"x * y = %.5f, x * z = %.5f, y * z = %.5f", vectorMultiply(_camera.xAxis, _camera.yAxis),vectorMultiply(_camera.xAxis, _camera.lookAtDirection), vectorMultiply(_camera.yAxis, _camera.lookAtDirection));
    
    [self setNeedsDisplay];
}

#pragma mark cam action

- (void)moveForward{
    float moveDistance = _moveSpeed;
    NEVector3 translation = GLKVector3Make(moveDistance * _camera.lookAtDirection.x, moveDistance * _camera.lookAtDirection.y, moveDistance * _camera.lookAtDirection.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}

- (void)moveBack{
    float moveDistance = - _moveSpeed;
    NEVector3 translation = GLKVector3Make(moveDistance * _camera.lookAtDirection.x, moveDistance * _camera.lookAtDirection.y, moveDistance * _camera.lookAtDirection.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}

- (void)moveUp{
    float moveDistance = _moveSpeed;
    NEVector3 translation = GLKVector3Make(moveDistance * _camera.yAxis.x, moveDistance * _camera.yAxis.y, moveDistance * _camera.yAxis.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}
- (void)moveDown{
    float moveDistance = - _moveSpeed;
    NEVector3 translation = GLKVector3Make(moveDistance * _camera.yAxis.x, moveDistance * _camera.yAxis.y, moveDistance * _camera.yAxis.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}
- (void)moveLeft{
    float moveDistance = _moveSpeed;
    NEVector3 translation = GLKVector3Make(moveDistance * _camera.xAxis.x, moveDistance * _camera.xAxis.y, moveDistance * _camera.xAxis.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}
- (void)moveRight{
    float moveDistance = - _moveSpeed;
    NEVector3 translation = GLKVector3Make(moveDistance * _camera.xAxis.x, moveDistance * _camera.xAxis.y, moveDistance * _camera.xAxis.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}

- (void)lookUp{
    float angleDiff = - _rotationRate;
//    _camera.yAxis = rotationByAngle(_camera.yAxis, _camera.xAxis, angleDiff);
//    _camera.lookAtDirection = rotationByAngle(_camera.lookAtDirection, _camera.xAxis, angleDiff);
    
    [_camera rotateByNearVerticallyByDegree:angleDiff];
    
    [self redraw];
}
- (void)lookDown{
    float angleDiff = _rotationRate;
//    _camera.yAxis = rotationByAngle(_camera.yAxis, _camera.xAxis, angleDiff);
//    _camera.lookAtDirection = rotationByAngle(_camera.lookAtDirection, _camera.xAxis, angleDiff);
    
    [_camera rotateByNearVerticallyByDegree:angleDiff];
    
    [self redraw];
}
- (void)turnLeft{
    float angleDiff = _rotationRate;
//    _camera.xAxis = rotationByAngle(_camera.xAxis, _camera.yAxis, angleDiff);
//    _camera.lookAtDirection = rotationByAngle(_camera.lookAtDirection, _camera.yAxis, angleDiff);
    
    [_camera rotateByNearHorizontallyByDegree:angleDiff];
    
    [self redraw];
}
- (void)turnRight{
    float angleDiff = - _rotationRate;
//    _camera.xAxis = rotationByAngle(_camera.xAxis, _camera.yAxis, angleDiff);
//    _camera.lookAtDirection = rotationByAngle(_camera.lookAtDirection, _camera.yAxis, angleDiff);
    
    [_camera rotateByNearHorizontallyByDegree:angleDiff];
    
    [self redraw];
}

- (void)loader:(NEAssLoader*)loader didLoadMesh:(NEMesh)mesh{
    
}

@end
