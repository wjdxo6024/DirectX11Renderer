#include "Model.h"

bool Model::init(DirectXEngine* DXEngine, FbxNode* root)
{
	externEngine = DXEngine;
	return true;
}

bool Model::Update()
{
	return true;
}

bool Model::Render(XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj, Light* light)
{
	for (int i = 0; i < m_ModelParts.size(); i++)
	{
		m_ModelParts[i]->Render(externEngine, world, view, proj, light);
	}
	return true;
}

void Model::Quit()
{

}

bool Model::PushModelPart(ModelPart* part)
{
	m_ModelParts.push_back(part);
	return true;
}

bool Model::CreateModelPart()
{
	return true;
}