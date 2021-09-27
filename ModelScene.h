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

// �� ���̶� �ϳ��� ���� �������� �޽��� ������ �ְ� �̸� �����ϱ� ����
// ��°�� ����̴�.

// Fbx������ ��� Ʈ���� �Ǿ� �ִ� �����̴�. ���� ��� ��忡 �ڽĳ�尡 �ִ��� Ȯ���ϸ鼭
// ��ȸ�� �ؾ� �Ѵ�.

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

	// ���� ������ �޾ƿ��� �ܺ� ��ҵ�
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