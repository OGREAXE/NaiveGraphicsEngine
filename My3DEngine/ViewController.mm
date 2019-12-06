//
//  ViewController.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "ViewController.h"
#import "NERenderView.h"
#import "NEAssLoader.h"
#import <math.h>

@interface ViewController () <NEAssLoaderDelegate>

@property (nonatomic) NERenderView *renderView;

@property (nonatomic) BOOL lineFrameMode;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    _lineFrameMode = YES;
    // Do any additional setup after loading the view.
    
//    _renderView = [[NERenderView alloc] initWithFrame:self.view.bounds];
    _renderView = [[NERenderView alloc] initWithFrame:CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.width)];
    _renderView.lineFrameMode = self.lineFrameMode;
    [self.view addSubview:_renderView];
    
    NEAssLoader * loader = [[NEAssLoader alloc] init];
    loader.delegate = self;
    
//    [loader loadDefaultFile];
    
    NELoadAssParam loadParam;
    loadParam.position = NEVector3Make(2, -2, 0);
    loadParam.rotation = NEVector3Make(M_PI_2, 0, 0);
    loadParam.width = 10;
    
    [loader loadFbx:@"duck" param:loadParam];
    
    [loader upload];
    
//    [loader loadFbx:@"Lamborghini_Aventador"];
//
//    _renderView.frameLines = [loader lineFrameFromMeshAt:0 range:20];
}


- (void)viewDidAppear:(BOOL)animated{
    _renderView.frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.width);
}

- (void)debugShowInfo{
        
    //    NEVector3 positionInCam = getPositionInCameraCoordinateSystem(NEVector3Make(2, 2, 1), NEVector3Make(1, 1, 0), NEVector3Make(0, 1, 0), NEVector3Make(0, 0, 1));
        //should be (-1, 1, 1)
    //    NSLog(@"result is %f, %f, %f",positionInCam.x, positionInCam.y, positionInCam.z);
        
        NEVector3 positionInCam = getPositionInCameraCoordinateSystem(NEVector3Make(3, 3, 2), NEVector3Make(2, 2, 0), NEVector3Make(1, 1, 1), NEVector3Make(-1, -1, 2));
        
        NSLog(@"result is %f, %f, %f",positionInCam.x, positionInCam.y, positionInCam.z);
}

- (void)loader:(NEAssLoader *)loader didLoadMeshes:(std::vector<NEMesh> &)meshes{
    
    if (_lineFrameMode) {
        _renderView.frameLines = [loader lineFrameFromMesh:meshes[0] range:20];
    } else {
        [_renderView loadMeshes:meshes];
    }
}

///////

- (IBAction)didTapMoveFoward{
    [_renderView moveForward];
}

- (IBAction)didTapMoveBack{
    [_renderView moveBack];
}

- (IBAction)didTapMoveUp{
    [_renderView moveUp];
}

- (IBAction)didTapMoveDown{
    [_renderView moveDown];
}

- (IBAction)didTapMoveLeft{
    [_renderView moveLeft];
}

- (IBAction)didTapMoveRight{
    [_renderView moveRight];
}

- (IBAction)didTapLookUp{
    [_renderView lookUp];
}

- (IBAction)didTapLookDown{
    [_renderView lookDown];
}

- (IBAction)didTapTurnLeft{
    [_renderView turnLeft];
}

- (IBAction)didTapTurnRight{
    [_renderView turnRight];
}

@end
