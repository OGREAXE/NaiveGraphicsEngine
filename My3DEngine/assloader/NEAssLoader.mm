//
//  NEAssLoader.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/19.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "NEAssLoader.h"
#include "CNEAssReader.hpp"

@interface NEAssLoader()

@property (nonatomic) CNEAssReader *reader;

@end

@implementation NEAssLoader

- (id)init{
    self = [super init];
    if (self) {
        [self doInit];
    }
    
    return self;
}

- (void)dealloc{
    delete _reader;
}

- (void)doInit{
    _reader = new CNEAssReader();
    
    [self loadFbx:@"box"];
}

- (void)loadFbx:(NSString *)modelName{
    NSString *fileName = [[NSBundle mainBundle] pathForResource:modelName ofType:@"fbx"];
    
    _reader->LoadMesh(fileName.UTF8String);
}

@end
