#pragma once
#ifndef _MODELSCENE_H_
#define _MODELSCENE_H_
#include "DXHeader.h"
#include "DirectXEngine.h"
#include "CustomUtility.h"

#include <fbxsdk.h>
#include "Model.h"
#include "ModelShaders.h"
#include "ModelSkeleton.h"
#include "ModelPart.h"
#include "ModelMaterial.h"
#include "ModelAnimationController.h"
#include "ModelBoneWeight.h"
#include "Light.h"
#include "Camera.h"

// 모델 씬이란 하나의 모델이 여러개의 메쉬를 가지고 있고 이를 조정하기 위한
// 통째의 장면이다.

// Fbx파일은 모든 트리로 되어 있는 구조이다. 따라서 모든 노드에 자식노드가 있는지 확인하면서
// 순회를 해야 한다.

class ModelScene
{
private:
	FbxManager* m_FbxManager = nullptr;
	FbxScene* m_FbxScene = nullptr;
	FbxGeometryConverter* m_FbxConverter = nullptr;
	FbxImporter* m_FbxImporter = nullptr;
	
	ModelAnimationController* m_AnimationController = nullptr;

	FbxSkeleton* skeleton;
	ModelSkeleton* m_Skeleton;

	Model* m_Model;

	DirectXEngine* externEngine;

	// 기존 씬에서 받아오는 외부 요소들
	Camera* externCamera;
	ModelShaders* externShaders;
	std::vector<Light*> externLights;
private:
	bool LoadModel(FbxScene* fbxscene, bool haveMesh, bool haveMaterial, bool haveAnimation, FbxNodeAttribute::EType attribute);

	bool LoadMaterial();
	bool LoadMesh(FbxNode* root, FbxNodeAttribute::EType attribute, bool IsAnimatd);
	bool LoadSkeleton(FbxNode* root, FbxNodeAttribute::EType attribute);
	bool LoadBoneWeights(FbxMesh* mesh, std::vector<ModelBoneWeight>& meshBoneWeights);
	bool LoadBoneWeightSkin(FbxSkin* skin, std::vector<ModelBoneWeight>& meshBoneWeights);
	bool LoadAnimation();

public:
	ModelScene();
	ModelScene(const ModelScene& other);
	~ModelScene();

	bool Init(DirectXEngine* dxEngine, Camera* camera, ModelShaders* shaders, const char* filePath);
	bool Render(XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj);
	bool Update();

	bool LoadScene(const char* filePath);
	bool ProcessScene();
	void Quit();

	void PushLight(Light* light);
};

#endif