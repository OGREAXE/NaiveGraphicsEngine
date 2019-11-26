//
//  NEAssLoader.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/19.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "NEAssLoader.h"
#include "CNEAssReader.hpp"
#import "NEMath.h"
#import "NEPolygonLine.h"

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
    
//    [self loadFbx:@"Cinema4D"];
}

- (void)loadDefaultFile{
    NEMesh mesh;
    
    NEVertice vx = { 1, 0, 0 };
    NEVertice vy = { 0, 2, 0 };
    NEVertice vz = { 0, 0, 3};
    
    mesh.vertices.push_back(vx);
    mesh.vertices.push_back(vy);
    mesh.vertices.push_back(vz);
    
    mesh.range = 20;
    
    NEFace face;
    face.aIndex = 0;
    face.bIndex = 1;
    face.cIndex = 2;
    
    face.color = 0xffff00;
    
    mesh.faces.push_back(face);
    
    std::vector<NEMesh> v;
    v.push_back(mesh);
    
    [self.delegate loader:self didLoadMeshes:v];
}

- (void)loadFbx:(NSString *)modelName{
    NSString *fileName = [[NSBundle mainBundle] pathForResource:modelName ofType:@"fbx"];
    
    if (_reader->LoadMesh(fileName.UTF8String)){
        [self.delegate loader:self didLoadMeshes:_reader->mMeshes];
    }
}

- (NSArray<NEPolygonLine*>*)lineFrameFromMeshAt:(int)meshIndex range:(float)range{
    NEMesh & mesh = _reader->mMeshes[meshIndex];
    
    return [self lineFrameFromMesh:mesh range:range];
}

- (NSArray<NEPolygonLine*>*)lineFrameFromMesh:(NEMesh &)mesh range:(float)range{
    
    
    NSMutableArray<NEPolygonLine*>* lines = [NSMutableArray array];
    
    for (NEFace & aFace : mesh.faces) {
        NEVertice &v0 = mesh.vertices[aFace.aIndex];
        NEVertice &v1 = mesh.vertices[aFace.bIndex];
        NEVertice &v2 = mesh.vertices[aFace.cIndex];
        
        NEVector3 vec0 = [self trimWithVertice:v0 max:mesh.range range:range];
        NEVector3 vec1 = [self trimWithVertice:v1 max:mesh.range range:range];
        NEVector3 vec2 = [self trimWithVertice:v2 max:mesh.range range:range];
        
        NEPolygonLine *line0 = [NEPolygonLine lineWithStart:vec0 end:vec1];
        
        NEPolygonLine *line1 = [NEPolygonLine lineWithStart:vec0 end:vec2];
        
        NEPolygonLine *line2 = [NEPolygonLine lineWithStart:vec1 end:vec2];
        
        [lines addObject:line0];
        [lines addObject:line1];
        [lines addObject:line2];
    }
    
    NSLog(@"faces count is %d", lines.count/3);
    
    return lines;
}

- (NEVector3)trimWithVertice:(NEVertice)vec max:(float)max range:(float)range{
    NEVector3 vecr;
    vecr.x = vec.x * range /max;
    vecr.y = vec.y * range /max;
    vecr.z = vec.z * range /max;
    return vecr;
}

@end
