#pragma once
#include "DXHeader.h"

#include <fbxsdk.h>
#include <vector>
#include <iostream>
// 현재 FBXLoader는 DX9으로 맞춰서 되어 있기 때문에 DX11로 포팅해서 할 것, 특히 Vertex정의 부분.

void LoadFbxWithAnimation(const char* filename, std::vector<ModelVertex>* Vertices, std::vector<DWORD>* Indices);

void LoadMeshInformation(FbxMesh* pMesh, std::vector<ModelVertex>* pOutVertexVector);

//-----------------------------------------------------------------------------------------------


void LoadUVInformation(FbxMesh* pMesh, std::vector<ModelVertex>* pOutVertexVector);
void LoadUVInformation(FbxMesh* pMesh, std::vector<ModelBumpVertex>* pOutVertexVector);
void LoadUVInformation(FbxMesh* pMesh, std::vector<ModelVertexForTest>* pOutVertexVector);

void LoadFBX(const char* filename, std::pair<int, int>* memberCounts, std::vector<ModelVertex>* Vertices, std::vector<DWORD>* Indices);
void LoadFBX(const char* filename, std::pair<int, int>* memberCounts, std::vector<ModelVertexForTest>* Vertices, std::vector<unsigned int>* Indices);

void LoadFBXwithBumpMap(const char* filename, std::pair<int, int>* memberCounts, std::vector<ModelBumpVertex>* Vertices, std::vector<unsigned int>* Indices);




//HRESULT LoadFBXwithAnimation(const char* filename,
//	DWORD MeshOptions,
//	ID3D11Device pDevice,
//	LPD3DXALLOCATEHIERARCHY pAlloc,
//	LPD3DXLOADUSERDATA pUserDataLoader,
//	LPD3DXFRAME* ppFrameHierarchy,
//	LPD3D10DXANIMATION* ppAnimController);