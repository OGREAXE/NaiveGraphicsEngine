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

//better not use std containers as attributes
//'cause getter(self.) returns a copy not reference
//@property (nonatomic) std::vector<NEMesh> meshes;

@end

@implementation NEAssLoader{
    std::vector<NEMesh> _meshes;
}

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

- (void)reset{
    _meshes.clear();
}

- (void)loadDefaultFile{
//    _meshes.clear();
    
//    [self loadPlanes];
    
    [self loadPlanes2WithOffsetX:2 offsetY:2 z:3.5 rotation:NEVector3Make(0, 0, 0)];
    
//    [self loadPlanes2WithOffsetX:2 offsetY:1.5 z:1];
    
//    [self loadTestMesh];
    
//    [self.delegate loader:self didLoadMeshes:_meshes];
}

- (void)loadPlanes{
    float planeRange = 10;
    NSArray * vertices = @[
    @[@(0), @(0) ,@(0)], @[@(0), @(1) ,@(0)], @[@(1), @(1) ,@(0)], @[@(1), @(0) ,@(0)], @[@(1), @(0) ,@(1)], @[@(0), @(0) ,@(1)], @[@(0), @(1) ,@(1)],];
    
    NEMesh mesh;
    mesh.range = planeRange;
    
    for (NSArray<NSNumber *> * arrVertice : vertices) {
        NEVertice v = { arrVertice[0].floatValue * planeRange, arrVertice[1].floatValue * planeRange, arrVertice[2].floatValue * planeRange };
        
        v.normal.z = 1;
        v.normal.y = 0;
        v.normal.x = 0;
        mesh.vertices.push_back(v);
    }
    
//    NSArray * indices = @[
//    @[@(0), @(1) ,@(2)], @[@(2), @(3) ,@(0)], //xy
//    @[@(0), @(3) ,@(4)], @[@(4), @(5) ,@(0)], //xz
//    @[@(0), @(5) ,@(6)], @[@(6), @(1) ,@(0)], //yz
//    ];
    
    NSArray * indices = @[
    @[@(0), @(1) ,@(2)],
  @[@(2), @(3) ,@(0)], //xy
//    @[@(0), @(3) ,@(4)], @[@(4), @(5) ,@(0)], //xz
//    @[@(0), @(5) ,@(6)], @[@(6), @(1) ,@(0)], //yz
    ];
    
    NSArray<NSNumber *> * colors = @[
    @(0xeeeeee), @(0xeeeeee), //xy
    @(0xaaaaaa), @(0xaaaaaa), //xz
    @(0x888888), @(0x888888), //yz
    ];
    
    for (int i = 0; i < indices.count; i++) {
        NSArray<NSNumber *> * aFaceIndices = indices[i];
        
        NEFace face;
        face.aIndex = aFaceIndices[0].intValue;
        face.bIndex = aFaceIndices[1].intValue;
        face.cIndex = aFaceIndices[2].intValue;
        
        face.color = colors[i].longValue;
        
        mesh.faces.push_back(face);
    }
    
    _meshes.push_back(mesh);
}

- (void)loadPlanes2WithOffsetX:(float)offsetx offsetY:(float)offsety z:(float)z rotation:(NEVector3)rotation{
    float planeRange = 1;
    NSArray * vertices = @[
    @[@(0), @(0) ,@(z)], @[@(0), @(1) ,@(z)], @[@(1), @(1) ,@(z)], @[@(1), @(0) ,@(z)]];
    
    NEMesh mesh;
    mesh.range = 1;
    mesh.width = 2;
    mesh.roatation = rotation;
    
//    float offsetx = 2;
//    float offsety = 2;
    
    for (NSArray<NSNumber *> * arrVertice : vertices) {
        NEVertice v = { (arrVertice[0].floatValue + offsetx) * planeRange, (arrVertice[1].floatValue  + offsety) * planeRange, arrVertice[2].floatValue * planeRange };
        
        v.normal.z = 1;
        v.normal.y = 0;
        v.normal.x = 0;
        mesh.vertices.push_back(v);
    }
    
//    NSArray * indices = @[
//    @[@(0), @(1) ,@(2)], @[@(2), @(3) ,@(0)], //xy
//    @[@(0), @(3) ,@(4)], @[@(4), @(5) ,@(0)], //xz
//    @[@(0), @(5) ,@(6)], @[@(6), @(1) ,@(0)], //yz
//    ];
    
    NSArray * indices = @[
    @[@(0), @(1) ,@(2)],
  @[@(2), @(3) ,@(0)], //xy
//    @[@(0), @(3) ,@(4)], @[@(4), @(5) ,@(0)], //xz
//    @[@(0), @(5) ,@(6)], @[@(6), @(1) ,@(0)], //yz
    ];
    
    NSArray<NSNumber *> * colors = @[
    @(0xeeeeee), @(0xeeeeee), //xy
    @(0xaaaaaa), @(0xaaaaaa), //xz
    @(0x888888), @(0x888888), //yz
    ];
    
    for (int i = 0; i < indices.count; i++) {
        NSArray<NSNumber *> * aFaceIndices = indices[i];
        
        NEFace face;
        face.aIndex = aFaceIndices[0].intValue;
        face.bIndex = aFaceIndices[1].intValue;
        face.cIndex = aFaceIndices[2].intValue;
        
        face.color = colors[i].longValue;
        
        mesh.faces.push_back(face);
    }
    
    _meshes.push_back(mesh);
}

- (void)loadTestMesh{
    NSArray * vertices = @[
    @[@(1), @(1) ,@(1)], @[@(1), @(3) ,@(1)], @[@(3), @(3) ,@(1)],@[@(3), @(1) ,@(1)],
    
    @[@(1), @(1) ,@(3)], @[@(1), @(3) ,@(3)], @[@(3), @(3) ,@(3)], @[@(3), @(1) ,@(3)],
    ];
    
    NEMesh mesh;
    mesh.range = 20;
    
    for (NSArray<NSNumber *> * arrVertice : vertices) {
        NEVertice v = { arrVertice[0].floatValue, arrVertice[1].floatValue, arrVertice[2].floatValue };
        
        mesh.vertices.push_back(v);
    }
    
    NSArray * indices = @[
    @[@(0), @(1) ,@(2)], @[@(2), @(3) ,@(0)], //bottom
    @[@(4), @(5) ,@(6)], @[@(6), @(7) ,@(4)], //top
    @[@(7), @(6) ,@(2)], @[@(2), @(3) ,@(7)], //front
    @[@(6), @(5) ,@(1)], @[@(1), @(2) ,@(6)], //right
    @[@(4), @(0) ,@(1)], @[@(1), @(5) ,@(4)], //back
    @[@(7), @(3) ,@(0)], @[@(0), @(4) ,@(7)], //left
    ];
    
    NSArray<NSNumber *> * colors = @[
    @(0xff0000), @(0xff0000), //bottom
    @(0x00ff00), @(0x00ff00), //top
    @(0x0000ff), @(0x0000ff), //front
    @(0xffff00), @(0xffff00), //right
    @(0x00ffff), @(0x00ffff), //back
    @(0xff00ff), @(0xff00ff), //left
    ];
    
    for (int i = 0; i < indices.count; i++) {
        NSArray<NSNumber *> * aFaceIndices = indices[i];
        
        NEFace face;
        face.aIndex = aFaceIndices[0].intValue;
        face.bIndex = aFaceIndices[1].intValue;
        face.cIndex = aFaceIndices[2].intValue;
        
        face.color = colors[i].longValue;
        
        mesh.faces.push_back(face);
    }
    
    _meshes.push_back(mesh);
}

- (void)loadFbx:(NSString *)modelName  param:(NELoadAssParam)param{
    NSString *fileName = [[NSBundle mainBundle] pathForResource:modelName ofType:@"fbx"];
    
    if (_reader->LoadMesh(fileName.UTF8String)){
        for (NEMesh & mesh : _reader->mMeshes) {
            mesh.position = param.position;
            mesh.roatation = param.rotation;
            mesh.width = param.width;
        }
        
        _meshes.insert(_meshes.end(), _reader->mMeshes.begin(), _reader->mMeshes.end());
    }
}

- (void)upload{
    [self.delegate loader:self didLoadMeshes:_meshes];
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
