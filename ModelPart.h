#pragma once
#include "DXHeader.h"
#include "ModelMesh.h"
#include "ModelAnimMesh.h"
#include "ModelMaterial.h"
#include "ModelBoneWeight.h"

#include "TextureArray.h"
#include "Light.h"
// Shader
#include "BumpMapShaderClass.h"
#include "ModelShaders.h"
#include <vector>

class ModelPart
{
private:
	int m_ParentIndex;
	int m_CurrentIndex;

	ModelMesh* currentMesh = nullptr;
	std::vector<ModelMesh*> m_ChildMeshs;

	ModelAnimMesh* currentAnimMesh = nullptr;
	std::vector<ModelAnimMesh*> m_ChildAnimMeshs;

	std::vector<ModelMaterial*> m_Materials;
	std::vector<ModelBoneWeight> boneWeights;

	XMMATRIX m_ModelTransform;

	bool isSkinnedModel;

	// ¿ÜºÎ
	ModelShaders* externShaders;
	DirectXEngine* externEngine;
private:
	void InitMesh(FbxNode* node);
	void ReadMaterial(FbxSurfaceMaterial* material);
	void ReadBoneWeight();

	void LoadChildNode(FbxNode* node);

public:
	bool Init(DirectXEngine* DXEngine, FbxNode* node, bool IsAnimated, ModelShaders* shaders);
	bool Render(DirectXEngine* DXEngine, XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj, Light* light);
	bool Update();
	void Quit();

	int GetChildCount();

	ModelAnimMesh* GetAnimChild(int index);
	ModelMesh* GetChild(int index);

};

