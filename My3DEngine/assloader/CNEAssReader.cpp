//
//  CNEAssReader.cpp
//  My3DEngine
//
//  Created by Liang,Zhiyuan(GIS)2 on 2019/11/19.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#include "CNEAssReader.hpp"
#include <vector>

bool CNEAssReader::LoadMesh(const std::string& Filename){
    bool Ret = false;
    
   Assimp::Importer Importer;

   const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

   if (pScene) {
       Ret = InitFromScene(pScene, Filename);
   }
   else {
       printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
   }

   return Ret;
}

bool CNEAssReader::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    
    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    return true;
    
}

void CNEAssReader::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{

//    std::vector Vertices;
//    std::vector Indices;
 
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    printf("numVertices %d", paiMesh->mNumVertices);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i])?: &Zero3D;
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

//        Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
//                Vector2f(pTexCoord->x, pTexCoord->y),
//                Vector3f(pNormal->x, pNormal->y, pNormal->z));
//
//        Vertices.push_back(v);
    }
    
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
//        Indices.push_back(Face.mIndices[0]);
//        Indices.push_back(Face.mIndices[1]);
//        Indices.push_back(Face.mIndices[2]);
    }
 
//    m_Entries[Index].Init(Vertices, Indices);
}
