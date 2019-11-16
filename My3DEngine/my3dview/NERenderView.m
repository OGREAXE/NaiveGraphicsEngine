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
//    camera.position = GLKVector3Make(2, 2, 2);
//    [camera lookAt:GLKVector3Make(-1, -1, -2)];
//    camera.yAxis = GLKVector3Make(-1, -1, 1);
    
    camera.position = GLKVector3Make(2, 2, 2);
    NEVector3 pointToLookAt = GLKVector3Make(0, 2, 0);
    [camera lookAt:GLKVector3Make(pointToLookAt.x - camera.position.x, pointToLookAt.y - camera.position.y, pointToLookAt.z - camera.position.z)];
//    [camera lookAt:GLKVector3Make(-5, -1, -3)];
    
    float yAxis_x = -1, yAxis_y = -1;
//    camera.yAxis = GLKVector3Make(-1, -1, 2);
    camera.yAxis = getVerticalVec(camera.lookAtDirection, &yAxis_x, &yAxis_y, NULL);
    
    self.camera = camera;
    
    NEPolygonLine * xAxis = [[NEPolygonLine alloc] init];
    xAxis.startPosition = GLKVector3Make(0, 0, 0);
    xAxis.endPosition = GLKVector3Make(100, 0, 0);
   
    NEPolygonLine * yAxis = [[NEPolygonLine alloc] init];
    yAxis.startPosition = GLKVector3Make(0, 0, 0);
    yAxis.endPosition = GLKVector3Make(0, 100, 0);
   
    NEPolygonLine * zAxis = [[NEPolygonLine alloc] init];
    zAxis.startPosition = GLKVector3Make(0, 0, 0);
    zAxis.endPosition = GLKVector3Make(0, 0, 100);
    
    self.xAxis = xAxis;
    self.yAxis = yAxis;
    self.zAxis = zAxis;
}

- (CGPoint)positionInView:(NEVector3)originalPoint{
    CGFloat width = self.frame.size.width;
    CGFloat height = self.frame.size.height;
    
    NEVector3 point = getPositionInCameraCoordinateSystem(originalPoint, self.camera.position, self.camera.lookAtDirection, self.camera.yAxis);
    
    NEVector3 point_tran = perspetiveProjectPoint(point, self.camera.frustum);
    
    CGFloat screen_x = (1 -  point_tran.x)* width/2;
    CGFloat screen_y = height - ( 1 +  point_tran.y)* height/2;
    
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
    CGPoint p0 = [self positionInView:line.startPosition];
    CGPoint p1 = [self positionInView:line.endPosition];
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetStrokeColorWithColor(context, [UIColor redColor].CGColor);

    // Draw them with a 2.0 stroke width so they are a bit more visible.
    CGContextSetLineWidth(context, 2.0f);
    
    CGContextSetStrokeColorWithColor(context, color.CGColor);

    CGContextMoveToPoint(context, p0.x, p0.y); //start at this point

    CGContextAddLineToPoint(context, p1.x, p1.y); //draw to this point

    // and now draw the Path!
    CGContextStrokePath(context);
}

- (void)drawRect:(CGRect)rect{
    [self drawOrigin];
    
    [self drawLine:self.xAxis color:[UIColor redColor]];
    [self drawLine:self.yAxis color:[UIColor greenColor]];
    [self drawLine:self.zAxis color:[UIColor blueColor]];
}

@end
