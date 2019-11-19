//
//  CNEAssReader.hpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/19.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#ifndef CNEAssReader_hpp
#define CNEAssReader_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

class CNEAssReader {
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
public:
    bool LoadMesh(const std::string& Filename);
    
    std::vector<NEMesh> mMeshes;
};

#endif /* CNEAssReader_hpp */
