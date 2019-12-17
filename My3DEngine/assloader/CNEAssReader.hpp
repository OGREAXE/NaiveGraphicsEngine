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
#include "NEMesh.h"

typedef struct tagLoadParam {
    NEVector3 position;
    NEVector3 rotation;
    float width;
    
    int materialIndexOffset;
    int textureIndexOffset;
} NELoadAssParam;

class CNEAssReader {
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene);
    
    int _materialIndexOffset;
    int _textureIndexOffset;
public:
    bool LoadFile(const std::string& Filename);
    
    bool LoadFile(const std::string& Filename, NELoadAssParam *ploadParam);
    
    std::vector<NEMesh> mMeshes;
    
    std::vector<NEMaterial> mMaterials;
    std::vector<NETexture> mTextures;
};

#endif /* CNEAssReader_hpp */
