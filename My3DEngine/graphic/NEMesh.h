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

typedef struct tagNEVertice{
    float x;
    float y;
    float z;
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
    
    float range;
} NEMesh;

typedef struct tagNEBoundingBox{
    int startX;
    int startY;
    int endX;
    int endY;
} NEBoundingBox;

#endif /* NEMesh_h */
