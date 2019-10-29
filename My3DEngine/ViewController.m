//
//  ViewController.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "ViewController.h"
#import "NEScene.h"

@interface ViewController ()

@property (nonatomic) NEScene * scene;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
//    NEVector3 positionInCam = getPositionInCameraCoordinateSystem(GLKVector3Make(2, 2, 1), GLKVector3Make(1, 1, 0), GLKVector3Make(0, 1, 0), GLKVector3Make(0, 0, 1));
    //should be (-1, 1, 1)
//    NSLog(@"result is %f, %f, %f",positionInCam.x, positionInCam.y, positionInCam.z);
    
    NEVector3 positionInCam = getPositionInCameraCoordinateSystem(GLKVector3Make(3, 3, 2), GLKVector3Make(2, 2, 0), GLKVector3Make(1, 1, 1), GLKVector3Make(-1, -1, 2));
    
    NSLog(@"result is %f, %f, %f",positionInCam.x, positionInCam.y, positionInCam.z);
}

- (void)createScene{
    _scene = [[NEScene alloc] init];
}

@end
