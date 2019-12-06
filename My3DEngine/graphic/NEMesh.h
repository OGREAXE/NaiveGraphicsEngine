//
//  NEMesh.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEMesh_h
#define NEMesh_h

#include <vector>
#include "NEMath.h"

typedef struct tagNEVertice{
    float x;
    float y;
    float z;
    
//    float normal_x;
//    float normal_y;
//    float normal_z;
    NEVector3 normal;
} NEVertice;

typedef struct tagNEFace{
    int aIndex;
    int bIndex;
    int cIndex;
    
    long color;
} NEFace;

typedef struct tagNEMesh{
    std::vector<NEVertice> vertices;
    std::vector<NEFace> faces;
    
    float range; //the max distance of vertices
    float width; //the width in world
    
    NEVector3 position;
    NEVector3 roatation;
} NEMesh;

typedef struct tagNEBoundingBox{
    int startX;
    int startY;
    int endX;
    int endY;
} NEBoundingBox;

#endif /* NEMesh_h */
