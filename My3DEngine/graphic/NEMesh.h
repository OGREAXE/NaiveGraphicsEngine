//
//  NEMesh.h
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef NEMesh_h
#define NEMesh_h

typedef struct tagNEVertice{
    float x;
    float y;
    float z;
} NEVertice;

typedef struct tagNEFace{
    int aIndex;
    int bIndex;
    int cIndex;
} NEFace;

typedef struct tagNEMesh{
    std::vector<NEVertice> vertices;
    std::vector<NEFace> faces;
    
    float range;
} NEMesh;

#endif /* NEMesh_h */
