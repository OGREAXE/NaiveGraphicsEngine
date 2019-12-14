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
    
    mMeshes.clear();
    mTextures.clear();
    
   Assimp::Importer Importer;

   const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

   if (pScene) {
       Ret = InitFromScene(pScene, Filename);
   } else {
       printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
   }

   return Ret;
}

bool CNEAssReader::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    printf("mNumMeshes %d \n", pScene->mNumMeshes);
    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    InitMaterials(pScene);
    
    return true;
    
}

void CNEAssReader::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
//    std::vector<NEVertice> vertices;
//    std::vector<NEFace> faces;
    
    NEMesh aMesh;
 
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    printf("numVertices %d \n", paiMesh->mNumVertices);

    aiVector3D* pos0 = &(paiMesh->mVertices[0]);
    float maxx = pos0->x, minx = pos0->x;
    float maxy = pos0->y, miny = pos0->y;
    float maxz = pos0->z, minz = pos0->z;
    
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i])?: &Zero3D;
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        
        NEVertice vertice = {pPos->x, pPos->y, pPos->z, pNormal->x, pNormal->y, pNormal->z, pTexCoord->x, pTexCoord->y};
        aMesh.vertices.push_back(vertice);
        
        if (maxx < pPos->x) {
            maxx = pPos->x;
        }
        if (minx > pPos->x) {
            minx = pPos->x;
        }
        if (maxy < pPos->y) {
            maxy = pPos->y;
        }
        if (miny > pPos->y) {
            miny = pPos->y;
        }
        if (maxz < pPos->z) {
            maxz = pPos->z;
        }
        if (minz > pPos->z) {
            minz = pPos->z;
        }
    }
    
    printf("x range: [%.5f, y:%.5f]", minx, maxx);
    printf("y range: [%.5f, y:%.5f]", miny, maxy);
    printf("z range: [%.5f, y:%.5f]", minz, maxz);
    
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& face = paiMesh->mFaces[i];
        assert(face.mNumIndices == 3);
//        Indices.push_back(Face.mIndices[0]);
//        Indices.push_back(Face.mIndices[1]);
//        Indices.push_back(Face.mIndices[2]);
        
        NEFace aFace;
        aFace.aIndex = face.mIndices[0];
        aFace.bIndex = face.mIndices[1];
        aFace.cIndex = face.mIndices[2];
        
        aFace.color = 0xffffff;
        aMesh.faces.push_back(aFace);
    }
    
#define abs(a) (a<0?-a:a)
#define absMax(a, b) (abs(a)>abs(b)?abs(a):abs(b))
    aMesh.hasTexture = paiMesh->HasTextureCoords(0);
    aMesh.textureIndex = paiMesh->mMaterialIndex;
    aMesh.range =
    absMax(absMax(absMax(absMax(absMax(maxx, minx), maxy), miny), maxz), minz);
    mMeshes.push_back(aMesh);
 
//    m_Entries[Index].Init(Vertices, Indices);
}

bool CNEAssReader::InitMaterials(const aiScene* pScene)
{
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
        
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;
            
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                NETexture texture;
                texture.path = Path.data;
                
                mTextures.push_back(texture);
                
            }
        }
        
        if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            aiString Path;
            
            if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                NETexture texture;
                texture.path = Path.data;
                
                mTextures.push_back(texture);
                
            }
        }
    }
    
    return true;
}
