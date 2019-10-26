//
//  ViewController.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "ViewController.h"
#import "MEScene.h"

@interface ViewController ()

@property (nonatomic) MEScene * scene;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)createScene{
    _scene = [[MEScene alloc] init];
}

@end
