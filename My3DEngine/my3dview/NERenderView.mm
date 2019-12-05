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

@interface NERenderView()

//@property (nonatomic) NSArray<NSValue*>* vectices;

@property (nonatomic) NECamera* camera;

@property (nonatomic) NEPolygonLine* xAxis;
@property (nonatomic) NEPolygonLine* yAxis;
@property (nonatomic) NEPolygonLine* zAxis;

//@property (nonatomic) NEPolygonLine* line0;

//@property (nonatomic) std::vector<NEMesh> meshes;

@property (nonatomic) NSArray<NEPolygonLine*> *geometries;

//@property (nonatomic) NEDepthBuffer depthBuffer;

//@property (nonatomic) NEVector2 lineBuffer[];

@end

#define MAX_LINE_BUF 4096

#define COORD_AMPLIFY_FACTOR 1

typedef long long RenderBufferType;

@implementation NERenderView{
    NEVector3 _lineBuffer[MAX_LINE_BUF];
    
    std::vector<NEMesh> _meshes;
    
    NEDepthBuffer _depthBuffer;
    
    RenderBufferType *_renderBuffer;
    
    int _renderBufferSize;
    
    NEVector3 _lightPos;
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
    
    _renderBuffer = (RenderBufferType *)malloc(sizeof(RenderBufferType) * self.frame.size.width * self.frame.size.height);
    
    _lightPos = GLKVector3MultiplyScalar (_camera.position, 3);
    
//    _lightPos = NEVector3Make(2, 2, 2);
//    _lightPos = NEVector3Make(0, 0, 2);
}

- (void)dealloc{
    if (_renderBuffer) {
        free(_renderBuffer);
    }
}

- (void)setFrameLines:(NSArray<NEPolygonLine *> *)frameLines{
    _frameLines = frameLines;
    [self initLineFrame];
}

- (void)initCamera{
    NECamera * camera = [[NECamera alloc] init];
        
    camera.position = NEVector3Make(28, 28, 8);
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

- (NEVector3)convertToCameraSpace:(NEVector3)originalPoint{
    return getPositionInCameraCoordinateSystem(originalPoint, self.camera.position, self.camera.lookAtDirection, self.camera.yAxis);
}

- (NEVector3)convertToEyeSpace:(NEVector3)originalPoint{
//    NEVector3 point = getPositionInCameraCoordinateSystem(originalPoint, self.camera.position, self.camera.lookAtDirection, self.camera.yAxis);
    
    NEVector3 point = [self convertToCameraSpace:originalPoint];
    
    NEVector3 pointTran;
    
    if (shouldTrimPoint(point, _camera.frustum)) {
//        pointTran.x = 2;
//        pointTran.y = 2;
//        return pointTran;
    }
    
    pointTran = perspetiveProjectPoint(point, self.camera.frustum);
    
    return pointTran;
}

- (NEVector3)convertVectorToCameraSpace:(NEVector3&)aVector originInEyeSpace:(NEVector3&)originPosT{
    NEVector3 vectorT = [self convertToEyeSpace:aVector];
    return NEVector3Subtract(vectorT, originPosT);
}

- (NEVector3)convertVectorToEyeSpace:(NEVector3&)aVector originInEyeSpace:(NEVector3&)originPosT{
    NEVector3 vectorT = [self convertToEyeSpace:aVector];
    return NEVector3Subtract(vectorT, originPosT);
}

- (NEVector3)convertVectorToCameraSpace:(NEVector3&)aVector originInCameraSpace:(NEVector3&)originPosT{
    NEVector3 vectorT = [self convertToCameraSpace:aVector];
    return NEVector3Subtract(vectorT, originPosT);
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
    NSDate *timeBefore = [NSDate date];
    
    [self doDrawRect:rect];
    
    NSDate *timeAfter = [NSDate date];
    NSTimeInterval executionTime = [timeAfter timeIntervalSinceDate:timeBefore];
//    NSLog(@"executionTime = %f", executionTime);
}

- (void)doDrawRect:(CGRect)rect{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextClearRect(context, self.bounds);
    
    _depthBuffer.resetSize();
    
    _renderBufferSize = 0;
    
    if (_lineFrameMode) {
        [self drawRect_lineFrame:rect];
    } else {
        [self drawMeshes:_meshes];
    }
    
//    [self drawAxis];
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
//    [self logPosition];
    
    [self setNeedsDisplay];
}

- (void)logPosition{
    NSLog(@"lookAtDirection %.5f, %.5f, %.5f", _camera.lookAtDirection.x, _camera.lookAtDirection.y, _camera.lookAtDirection.z);
        
        NSLog(@" y axis %.5f, %.5f, %.5f", _camera.yAxis.x, _camera.yAxis.y, _camera.yAxis.z);
        
        NSLog(@" x axis %.5f, %.5f, %.5f", _camera.xAxis.x, _camera.xAxis.y, _camera.xAxis.z);
        
        NSLog(@" position %.5f, %.5f, %.5f", _camera.position.x, _camera.position.y, _camera.position.z);
        
        NSLog(@"--------------------------");
        
    //    NSLog(@"x * y = %.5f, x * z = %.5f, y * z = %.5f", vectorMultiply(_camera.xAxis, _camera.yAxis),vectorMultiply(_camera.xAxis, _camera.lookAtDirection), vectorMultiply(_camera.yAxis, _camera.lookAtDirection));
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

NEVector3 vectorFromVertice(const NEVertice & vert, float max, float range){
    NEVector3 v = {vert.x * range / max, vert.y * range / max, vert.z * range / max};
    return v;
}

inline bool isPointInsideTriangle_(CGPoint &point, NEVector2 &p0, NEVector2 &p1, NEVector2 &p2){
    NEVector2 p = NEVector2Make(point.x, point.y);
    
    return pointInsizeTriangle(p, p0, p1, p2);
}

inline bool isPointInsideTriangle(CGPoint &point, NEVector2 &p0, NEVector2 &p1, NEVector2 &p2){
    NEVector2 p = NEVector2Make(point.x, point.y);
    
#define testPoint(point, start, end) ((point.x - start.x) * (end.y - start.y) - (end.x - start.x) * (point.y - start.y));
    
    int t0 = testPoint(p, p0, p1);
    int t1 = testPoint(p, p1, p2);
    int t2 = testPoint(p, p2, p0);
    
    if ((t0 >= 0 && t1 >= 0 && t2 >= 0) ||
    (t0 <= 0 && t1 <= 0 && t2 <= 0)){
        return true;
    }
    
    return false;
}

#pragma mark ass loader

- (void)loadMeshes:(std::vector<NEMesh> &)meshes{
    _meshes = meshes;
}

float maxIntensity = 0;
float minIntensity = 1;

- (void)drawMeshes:(const std::vector<NEMesh> &)meshes{
    for (int i = 0; i < meshes.size(); i++) {
        const NEMesh & mesh = meshes[i];
        [self drawMesh:mesh];
    }
    
    [self doRenderScreen];
    
//    NSLog(@"max intensity %f, min intensity %f", maxIntensity, minIntensity);
}

inline long colorWithIntensity(long color, float intensity){
    if (intensity > maxIntensity) {
        maxIntensity = intensity;
    }
    
    if (intensity < minIntensity) {
        minIntensity = intensity;
    }
    
    long r = (color >>16) & 0xff;
    long g = (color >>8) & 0xff;
    long b = color & 0xff;
    
    r *= intensity;
    g *= intensity;
    b *= intensity;
    
    if (intensity < 0.01){
        int k = 0;
    }
    
    return r <<16 | g <<8 | b;
}

inline NEVector3 mixNormal(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2){
    return NEVector3Make((x0 + x1 + x2)/3., (y0 + y1 + y2)/3., (z0 + z1 + z2)/3.);
}

- (void)testInvertProject:(const NEMesh &)mesh{
    const NEVertice & _v0 = mesh.vertices[0];
    NEVector3 v0 = vectorFromVertice(_v0, mesh.range, 10);
    
    NEVector3 v0t = [self convertToEyeSpace:v0];
    
    NEVector3 v0c = [self convertToCameraSpace:v0];
    NEVector3 v0c_r = invertPerspetiveProject(v0t, self.camera.frustum);
    
    NEVector3 subtractRes = NEVector3Subtract(v0c, v0c_r);
    NSLog(@"testInvertProject: %f, %f, %f", subtractRes.x, subtractRes.y, subtractRes.z);
}

- (void)drawMesh:(const NEMesh &)mesh{
    NSLog(@"mesh draw begin >>>>>>>>>>>>>>");
//    CGContextRef context = UIGraphicsGetCurrentContext();
//    CGFloat fillWidth = 1./COORD_AMPLIFY_FACTOR;
    
    CGFloat screenWidth = self.frame.size.width * COORD_AMPLIFY_FACTOR;
    CGFloat screenHeight = self.frame.size.height * COORD_AMPLIFY_FACTOR;
    
    NEVector3 lightPosT = [self convertToEyeSpace:_lightPos];
    NEVector3 originPosT = [self convertToEyeSpace:NEVector3Make(0, 0, 0)];
    
    NEVector3 lightPosC = [self convertToCameraSpace:_lightPos];
    NEVector3 originPosC = [self convertToCameraSpace:NEVector3Make(0, 0, 0)];
    
    //do invert projection test here
//    [self testInvertProject:mesh];
//    return;
    //end test
    
    for (const NEFace & aface : mesh.faces) {
        long long color = aface.color;
//        CGContextSetFillColorWithColor(context, HEXRGBCOLOR(aface.color).CGColor);
        
        const NEVertice & _v0 = mesh.vertices[aface.aIndex];
        const NEVertice & _v1 = mesh.vertices[aface.bIndex];
        const NEVertice & _v2 = mesh.vertices[aface.cIndex];
        
        NEVector3 v0 = vectorFromVertice(_v0, mesh.range, 10);
        NEVector3 v1 = vectorFromVertice(_v1, mesh.range, 10);
        NEVector3 v2 = vectorFromVertice(_v2, mesh.range, 10);
        
        NEVector3 _normal =
        mixNormal(
                  _v0.normal_x, _v0.normal_y, _v0.normal_z,
                  _v1.normal_x, _v1.normal_y, _v1.normal_z,
                  _v2.normal_x, _v2.normal_y, _v2.normal_z);
        
        NEVector3 v0t = [self convertToEyeSpace:v0];
        NEVector3 v1t = [self convertToEyeSpace:v1];
        NEVector3 v2t = [self convertToEyeSpace:v2];
        
        CGPoint pointInVew0 = [self pointInVewForVector3:v0t];
        CGPoint pointInVew1 = [self pointInVewForVector3:v1t];
        CGPoint pointInVew2 = [self pointInVewForVector3:v2t];
        
        NEVector2 v0InView = NEVector2Make(pointInVew0.x, pointInVew0.y);
        NEVector2 v1InView = NEVector2Make(pointInVew1.x, pointInVew1.y);
        NEVector2 v2InView = NEVector2Make(pointInVew2.x, pointInVew2.y);
        
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
        
        //normalReal is the normal calculated from 3 vertices
        NEVector3 normalRealt = getPlaneNormal(v0t, v1t, v2t);
        
        //preDefined Normal affects light blend effect
//        NEVector3 preDefinedNormalt = [self convertVectorToEyeSpace:_normal originInEyeSpace:originPosT];
        
        NEVector3 preDefinedNormalC = [self convertVectorToCameraSpace:_normal originInCameraSpace:originPosC];
        
        CGFloat reverseHorizontalFactor = (1 / (screenWidth/2));
        CGFloat reverseVerticalFactor = (1 / (screenHeight/2));
        
        //////test angle
        
//        NEVector3 v0c = [self convertToCameraSpace:v0];
//        NEVector3 v1c = [self convertToCameraSpace:v1];
//        NEVector3 v2c = [self convertToCameraSpace:v2];
//
//        float angleT0 = getLightToPointAngle(v0c, lightPosC, preDefinedNormalC);
//        float angleT1 = getLightToPointAngle(v1c, lightPosC, preDefinedNormalC);
//        float angleT2 = getLightToPointAngle(v2c, lightPosC, preDefinedNormalC);
//
//        NEVector3 v0c_r = invertPerspetiveProject(v0t, self.camera.frustum);
//        NEVector3 v1c_r = invertPerspetiveProject(v1t, self.camera.frustum);
//        NEVector3 v2c_r = invertPerspetiveProject(v2t, self.camera.frustum);
//
//        NSLog(@"angle0 is %f, angle1 is %f, angle2 is %f", angleT0,  angleT1,  angleT2);
        
        /////end test angle
        
        for (int y = boundingBox.startY; y <= boundingBox.endY; y ++) {
            for (int x = boundingBox.startX; x <= boundingBox.endX; x ++) {
                CGPoint p = CGPointMake(x, y);
                //performance bottleneck
                if (!isPointInsideTriangle(p, v0InView, v1InView, v2InView)) {
                    continue;
                }
                    
                CGFloat revertX = revertScreenHorizatalPos(x, reverseHorizontalFactor);
                CGFloat revertY = revertScreenVerticalPos(y, reverseVerticalFactor);
                
                //the point in eye space inside the triangle
                NEVector3 point = getPointInPlane(revertX, revertY, normalRealt, v0t);
                
                NEVector3 pointc = invertPerspetiveProject(point, self.camera.frustum);
                
                /////handle light
//                long tColor = lightBlendResultWithColor(color, point, lightPosT, preDefinedNormalt);
                
                long tColor = lightBlendResultWithColor(color, pointc, lightPosC, preDefinedNormalC);
                /////finish handle color
                
                if (x > _depthBuffer.getWidth() || x < 0 || y > _depthBuffer.getHeight() || y < 0) {
                    continue;
                }
                
                DepthInfo info = _depthBuffer.getInfo(x, y);
                float oldZ = info.z;
                
#define COMPOSE_RENDER_BUF_VAL(x, y, color) ((x | (y << 16)) | (color << 32))
                if (point.z < oldZ) {
                    long oldIndex = info.additionalInfo;
                    if (oldIndex > 0) {
                        _renderBuffer[-- oldIndex] = COMPOSE_RENDER_BUF_VAL(x, y, tColor)
                        ;
                    } else {
                        _renderBuffer[_renderBufferSize ++] = COMPOSE_RENDER_BUF_VAL(x, y, tColor);
                        
                        info.additionalInfo = _renderBufferSize;
                    }
                    
//                    CGRect fillRect = CGRectMake(x / COORD_AMPLIFY_FACTOR - fillWidth/2, y / COORD_AMPLIFY_FACTOR - fillWidth/2, fillWidth, fillWidth);
//
//                    CGContextClearRect(context, fillRect);
//                    CGContextFillRect(context, fillRect);
                    
                    info.z = point.z;
                    info.color = tColor;
                    
                    _depthBuffer.setInfo(info, x, y);
                } else {
//                    int i = 0;
                }
            }
        }
        
        
    }
}

inline float getLightToPointAngle(NEVector3 & point, NEVector3 & lightPosT, NEVector3 & preDefinedNormal){
    NEVector3 lightOnPointVec = NEVector3Make(point.x - lightPosT.x, point.y - lightPosT.y, point.z - lightPosT.z);
    float lightAngle = getAngleBetweenVectors(lightOnPointVec, preDefinedNormal);

    return lightAngle;
}

inline long lightBlendResultWithColor(long color, NEVector3 & point, NEVector3 & lightPosT, NEVector3 & preDefinedNormal){
    float lightAngle = getLightToPointAngle(point, lightPosT, preDefinedNormal);
    
    if(lightAngle < M_PI_2){
        // smaller than pi/2, light is from behind
        lightAngle = M_PI_2;
    }
    
    long tColor = colorWithIntensity(color,  (1. - cosf(lightAngle))/2.);
    
    return tColor;
}

- (void)doRenderScreen{
//    return;
    
    CGFloat fillWidth = 1./COORD_AMPLIFY_FACTOR;
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    long lastColor = 0;
    for (int i = 0; i < _renderBufferSize; i++) {
        RenderBufferType renderValue = _renderBuffer[i];
        
        int x = renderValue & 0xffff;
        int y = (renderValue >> 16) & 0xffff;
        long renderColor = (renderValue >> 32);
        
        CGRect fillRect = CGRectMake(x / COORD_AMPLIFY_FACTOR - fillWidth/2, y / COORD_AMPLIFY_FACTOR - fillWidth/2, fillWidth, fillWidth);
        
        if (renderColor != lastColor) {
            lastColor = renderColor;
//            CGContextSetFillColorWithColor(context, HEXRGBCOLOR(renderColor).CGColor);
        }
        CGContextSetFillColorWithColor(context, HEXRGBCOLOR(renderColor).CGColor);
        CGContextFillRect(context, fillRect);
    }
}

@end
