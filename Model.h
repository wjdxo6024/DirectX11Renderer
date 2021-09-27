#pragma once
#include "ModelPart.h"
#include "ModelMaterial.h"
#include "DXHeader.h"
#include "DirectXEngine.h"
#include "Light.h"
class Model
{
private:
	std::vector<ModelPart*> m_ModelParts;
	DirectXEngine* externEngine;

public:
	bool init(DirectXEngine* DXEngine, FbxNode* root);
	bool Update();
	bool Render(XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj, Light* light);
	void Quit();

	bool PushModelPart(ModelPart* part);
	bool CreateModelPart();
};
