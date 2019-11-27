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

@property (nonatomic) std::vector<NEMesh> meshes;

@property (nonatomic) NSArray<NEPolygonLine*> *geometries;

@property (nonatomic) NEDepthBuffer depthBuffer;

//@property (nonatomic) NEVector2 lineBuffer[];

@end

#define MAX_LINE_BUF 4096

#define COORD_AMPLIFY_FACTOR 1

@implementation NERenderView{
    NEVector3 _lineBuffer[MAX_LINE_BUF];
    
    std::vector<NEMesh> _meshes;
    
    NEDepthBuffer _depthBuffer;
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
    
    [self initAxis];
    
//    [self initLineFrame];
    
    _depthBuffer.resetSize(self.frame.size.width * COORD_AMPLIFY_FACTOR, self.frame.size.height * COORD_AMPLIFY_FACTOR);
}

- (void)setFrameLines:(NSArray<NEPolygonLine *> *)frameLines{
    _frameLines = frameLines;
    [self initLineFrame];
}

- (void)initCamera{
    NECamera * camera = [[NECamera alloc] init];
        
    camera.position = NEVector3Make(7, 7, 2);
    NEVector3 pointToLookAt = NEVector3Make(0, 0, 2);
    
//    camera.position = NEVector3Make(-0, 12, 0);
//    NEVector3 pointToLookAt = NEVector3Make(0, 1, 0);
    
//    camera.position = NEVector3Make(0, 10, 0);
//    NEVector3 pointToLookAt = NEVector3Make(0, 12, 0);
    
    [camera lookAt:NEVector3Make(pointToLookAt.x - camera.position.x, pointToLookAt.y - camera.position.y, pointToLookAt.z - camera.position.z)];
    //find y axis which together with z form a plane vertical to xy plane
    NEVector3 lookAtRotated_90= { - camera.lookAtDirection.y , camera.lookAtDirection.x, 0};
    NEVector3 cam_yAxis = vectorCrossProduct(camera.lookAtDirection, lookAtRotated_90);
    camera.yAxis = cam_yAxis;
    camera.xAxis = lookAtRotated_90;
    
    [camera normalize];
    
    self.camera = camera;
    
    self.moveSpeed = 2;
    
//    self.rotationRate = M_PI_2/100;
    
    self.rotationRate = M_PI_2 /32;
}

- (void)initLineFrame{
    self.geometries = self.frameLines;
}

- (void)initAxis{
    NEPolygonLine * xAxis = [[NEPolygonLine alloc] init];
     xAxis.startPosition = NEVector3Make(0, 0, 0);
     xAxis.endPosition = NEVector3Make(5, 0, 0);
    
     NEPolygonLine * yAxis = [[NEPolygonLine alloc] init];
     yAxis.startPosition = NEVector3Make(0, 0, 0);
     yAxis.endPosition = NEVector3Make(0, 5, 0);
    
     NEPolygonLine * zAxis = [[NEPolygonLine alloc] init];
     zAxis.startPosition = NEVector3Make(0, 0, 0);
     zAxis.endPosition = NEVector3Make(0, 0, 5);
     
     self.xAxis = xAxis;
     self.yAxis = yAxis;
     self.zAxis = zAxis;
}

- (CGPoint)positionInView:(NEVector3)originalPoint{
    return [self positionInView:originalPoint projectResultNDC:nil];
}

- (NEVector3)convertToEyeSpace:(NEVector3)originalPoint{
    NEVector3 pointTran;
    
    NEVector3 point = getPositionInCameraCoordinateSystem(originalPoint, self.camera.position, self.camera.lookAtDirection, self.camera.yAxis);
    
    if (shouldTrimPoint(point, _camera.frustum)) {
        pointTran.x = 2;
        pointTran.y = 2;
        return pointTran;
    }
    
    pointTran = perspetiveProjectPoint(point, self.camera.frustum);
    
    return pointTran;
}

- (CGPoint)positionInView:(NEVector3)originalPoint projectResultNDC:(NEVector3 *)pointTran{
    CGFloat width = self.frame.size.width * COORD_AMPLIFY_FACTOR;
    CGFloat height = self.frame.size.height * COORD_AMPLIFY_FACTOR;
    
    NEVector3 point = getPositionInCameraCoordinateSystem(originalPoint, self.camera.position, self.camera.lookAtDirection, self.camera.yAxis);
    
    if (shouldTrimPoint(point, _camera.frustum)) {
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

//convert screen position (i.e 375 for iphone) to eye space [-1, 1]
- (CGFloat)revertScreenHorizontalPosition:(int)screenx{
    CGFloat width = self.frame.size.width * COORD_AMPLIFY_FACTOR;
    
    CGFloat ret = (screenx / (width/2) - 1);
    return ret;
}

- (CGFloat)revertScreenVerticalPosition:(int)screeny{
    CGFloat height = self.frame.size.height * COORD_AMPLIFY_FACTOR;
    
    CGFloat ret = (1 - screeny / (height/2));
    return ret;
}

static inline CGFloat revertScreenHorizatalPos(int screenX, CGFloat reverseFactor){
    return (screenX * reverseFactor - 1);
}

static inline CGFloat revertScreenVerticalPos(int screenY, CGFloat reverseFactor){
    return (1 - screenY * reverseFactor);
}

- (void)drawOrigin{
    CGPoint p0 = [self positionInView:NEVector3Make(0, 0, 0)];
    
    CGContextRef context = UIGraphicsGetCurrentContext();

    // Draw them with a 2.0 stroke width so they are a bit more visible.
    CGContextSetFillColorWithColor(context, UIColor.whiteColor.CGColor);

    CGContextAddEllipseInRect(context, CGRectMake(p0.x - 5, p0.y - 5, 10, 10));
    // and now draw the Path!
    CGContextFillPath(context);
}

- (void)drawLine:(NEPolygonLine *)line color:(long)color {
    [self drawPointsForLine:line color:color];
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

    CGContextSetFillColorWithColor(context, HEXRGBCOLOR(color).CGColor);
    
    int bufSize;
    
    getPointsArrayInLine(p0Tran, p1Tran, _lineBuffer, MAX_LINE_BUF, &bufSize);
    
    if (bufSize >= MAX_LINE_BUF) {
        return;
    }
    
    int last_posx = -100000, last_posy = -100000;
    
    for (int i = 0; i < bufSize; i++) {
        NEVector3 point = _lineBuffer[i];
        CGPoint pointInVew = [self pointInVewForVector3:point];
        
        int posx = (int)pointInVew.x, posy = (int)pointInVew.y;
        
        if (posx == last_posx && posy == last_posy) {
            continue;
        }
        last_posx = posx;
        last_posy = posy;
        
        if (posx > _depthBuffer.getWidth() || posx < 0 || posy > _depthBuffer.getHeight() || posy < 0) {
            continue;
        }
        
        DepthInfo info = _depthBuffer.getInfo(posx, posy);
        float oldZ = info.z;
        if (point.z < oldZ) {
//            CGContextSetFillColorWithColor(context, HEXRGBCOLOR(color).CGColor);
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
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextClearRect(context, self.bounds);
    
    _depthBuffer.resetSize();
    
    if (_lineFrameMode) {
        [self drawRect_lineFrame:rect];
    } else {
        const std::vector<NEMesh> & meshes = self.meshes;
        [self drawMeshes:meshes];
    }
    
    [self drawAxis];
}

- (void)drawRect_lineFrame:(CGRect)rect{
//    [self drawOrigin];
//    _depthBuffer.resetSize();
    
    for (NEPolygonLine * line in _geometries) {
        [self drawLine:line color:0xffff00]; //yellow
    }
}

- (void)drawAxis{
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
    NEVector3 translation = NEVector3Make(moveDistance * _camera.lookAtDirection.x, moveDistance * _camera.lookAtDirection.y, moveDistance * _camera.lookAtDirection.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}

- (void)moveBack{
    float moveDistance = - _moveSpeed;
    NEVector3 translation = NEVector3Make(moveDistance * _camera.lookAtDirection.x, moveDistance * _camera.lookAtDirection.y, moveDistance * _camera.lookAtDirection.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}

- (void)moveUp{
    float moveDistance = _moveSpeed;
    NEVector3 translation = NEVector3Make(moveDistance * _camera.yAxis.x, moveDistance * _camera.yAxis.y, moveDistance * _camera.yAxis.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}
- (void)moveDown{
    float moveDistance = - _moveSpeed;
    NEVector3 translation = NEVector3Make(moveDistance * _camera.yAxis.x, moveDistance * _camera.yAxis.y, moveDistance * _camera.yAxis.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}
- (void)moveLeft{
    float moveDistance = _moveSpeed;
    NEVector3 translation = NEVector3Make(moveDistance * _camera.xAxis.x, moveDistance * _camera.xAxis.y, moveDistance * _camera.xAxis.z);
    _camera.position = translationByVector(_camera.position, translation);
    
    [self redraw];
}
- (void)moveRight{
    float moveDistance = - _moveSpeed;
    NEVector3 translation = NEVector3Make(moveDistance * _camera.xAxis.x, moveDistance * _camera.xAxis.y, moveDistance * _camera.xAxis.z);
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

//util methods
NEVector3 vectorFromVertice(const NEVertice & vert){
    NEVector3 v = {vert.x, vert.y, vert.z};
    return v;
}

bool isPointInsideTriangle(CGPoint point, CGPoint p0, CGPoint p1, CGPoint p2){
    NEVector2 p = NEVector2Make(point.x, point.y);
    NEVector2 v0 = NEVector2Make(p0.x, p0.y);
    NEVector2 v1 = NEVector2Make(p1.x, p1.y);
    NEVector2 v2 = NEVector2Make(p2.x, p2.y);
    return pointInsizeTriangle(p, v0, v1, v2);
}

#pragma mark ass loader

- (void)loadMeshes:(std::vector<NEMesh> &)meshes{
    _meshes = meshes;
}

- (void)drawMeshes:(const std::vector<NEMesh> &)meshes{
    for (int i = 0; i < meshes.size(); i++) {
        const NEMesh & mesh = meshes[i];
        [self drawMesh:mesh];
    }
}

- (void)drawMesh:(const NEMesh &)mesh{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGFloat fillWidth = 1./COORD_AMPLIFY_FACTOR;
    
    CGFloat screenWidth = self.frame.size.width * COORD_AMPLIFY_FACTOR;
    CGFloat screenHeight = self.frame.size.height * COORD_AMPLIFY_FACTOR;
    
    for (const NEFace & aface : mesh.faces) {
        CGContextSetFillColorWithColor(context, HEXRGBCOLOR(aface.color).CGColor);
//        CGContextSetFillColorWithColor(context, UIColor.redColor.CGColor);
        
        const NEVertice & _v0 = mesh.vertices[aface.aIndex];
        const NEVertice & _v1 = mesh.vertices[aface.bIndex];
        const NEVertice & _v2 = mesh.vertices[aface.cIndex];
        
        NEVector3 v0 = vectorFromVertice(_v0);
        NEVector3 v1 = vectorFromVertice(_v1);
        NEVector3 v2 = vectorFromVertice(_v2);
        
        NEVector3 v0t = [self convertToEyeSpace:v0];
        NEVector3 v1t = [self convertToEyeSpace:v1];
        NEVector3 v2t = [self convertToEyeSpace:v2];
        
        CGPoint pointInVew0 = [self pointInVewForVector3:v0t];
        CGPoint pointInVew1 = [self pointInVewForVector3:v1t];
        CGPoint pointInVew2 = [self pointInVewForVector3:v2t];
        
        float minx = MIN(MIN(pointInVew0.x, pointInVew1.x), pointInVew2.x);
        float miny = MIN(MIN(pointInVew0.y, pointInVew1.y), pointInVew2.y);
        
        if (minx < 0) {minx = 0;}
        if (miny < 0) {miny = 0;}
        
        float maxx = MAX(MAX(pointInVew0.x, pointInVew1.x), pointInVew2.x);
        float maxy = MAX(MAX(pointInVew0.y, pointInVew1.y), pointInVew2.y);
        
        if (maxx > screenWidth) {maxx = screenWidth;}
        if (maxy > screenHeight) {maxy = screenHeight;}
        
        NEBoundingBox boundingBox;
        boundingBox.startX = (int)minx;
        boundingBox.startY = (int)miny;
        
        boundingBox.endX = (int)maxx;
        boundingBox.endY = (int)maxy;
        
        NEVector3 normal = getPlaneNormal(v0t, v1t, v2t);
        
        CGFloat reverseHorizontalFactor = (1 / (screenWidth/2));
        CGFloat reverseVerticalFactor = (1 / (screenHeight/2));
        
        for (int y = boundingBox.startY; y <= boundingBox.endY; y ++) {
            for (int x = boundingBox.startX; x <= boundingBox.endX; x ++) {
                
                if (!isPointInsideTriangle(CGPointMake(x, y), pointInVew0, pointInVew1, pointInVew2)) {
                    continue;
                }
                
//                CGFloat revertX = [self revertScreenHorizontalPosition:x];
//                CGFloat revertY = [self revertScreenVerticalPosition:y];
                
                CGFloat revertX = revertScreenHorizatalPos(x, reverseHorizontalFactor);
                CGFloat revertY = revertScreenVerticalPos(y, reverseVerticalFactor);
                
                NEVector3 point = getPointInPlane(revertX, revertY, normal, v0t);
                
                if (x > _depthBuffer.getWidth() || x < 0 || y > _depthBuffer.getHeight() || y < 0) {
                    continue;
                }
                
                DepthInfo info = _depthBuffer.getInfo(x, y);
                float oldZ = info.z;
                if (point.z < oldZ) {
                    
                    CGRect fillRect = CGRectMake(x / COORD_AMPLIFY_FACTOR - fillWidth/2, y / COORD_AMPLIFY_FACTOR - fillWidth/2, fillWidth, fillWidth);
                    
                    CGContextClearRect(context, fillRect);
                    CGContextFillRect(context, fillRect);
                    info.z = point.z;
                    info.color = aface.color;
                    _depthBuffer.setInfo(info, x, y);
                } else {
//                    int i = 0;
                }
            }
        }
    }
}

@end
