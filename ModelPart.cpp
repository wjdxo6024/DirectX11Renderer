#include "ModelPart.h"

bool ModelPart::Init(DirectXEngine* DXEngine, FbxNode* node, bool IsAnimated, ModelShaders* shaders)
{
	// �ܺ� ������
	externEngine = DXEngine;
	externShaders = shaders;
	
	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��.
	FbxGeometryConverter geometryConverter(node->GetFbxManager());
	geometryConverter.Triangulate(node->GetNodeAttribute(), true, false);

	isSkinnedModel = IsAnimated;
	if (isSkinnedModel) // AnimMesh
	{
		// Current Mesh Init
		currentAnimMesh = new ModelAnimMesh();
		currentAnimMesh->Init(externEngine->GetDevice(), externEngine->GetDeviceContext(), node->GetMesh());
		// Child Node
		int ChildCount = node->GetChildCount();
		if (ChildCount > 0) // Child�� �����Ѵ�.
		{
			for (int i = 0; i < ChildCount; i++)
			{
				FbxNode* childNode = node->GetChild(i);
				
				if (childNode->GetNodeAttribute() == NULL)
					continue;

				if (childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) // �޽��� �����Ѵ�.
				{
					ModelAnimMesh* childMesh = new ModelAnimMesh();

				}
			}
		}
	}
	else // Mesh
	{
		// Current Mesh Init
		currentMesh = new ModelMesh();
		currentMesh->Init(externEngine->GetDevice(), externEngine->GetDeviceContext(), node->GetMesh());
		
		// Child Node
		int ChildCount = node->GetChildCount();
		if (ChildCount > 0) // Child�� �����Ѵ�.
		{
			for (int i = 0; i < ChildCount; i++)
			{
				FbxNode* childNode = node->GetChild(i);

				if (childNode->GetNodeAttribute() == NULL)
					continue;

				if (childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) // �޽��� �����Ѵ�.
				{
					ModelMesh* childMesh = new ModelMesh();
				}
			}
		}
	}
	return true;
}

bool ModelPart::Render(DirectXEngine* DXEngine, XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj, Light* light)
{	
	// ���� ������ ������ �������� ����
	if (currentMesh != NULL)
	{
		currentMesh->Render(DXEngine->GetDeviceContext());

		// �ӽ� ����
		externShaders->RenderBump(DXEngine->GetDeviceContext(), currentMesh->GetindexCount(), world, view, proj, currentMesh->GetTextureArray(),
			light->GetDirection(), light->GetDiffuseColor());
	
		/*externShaders->RenderSpec(DXEngine->GetDeviceContext(), currentMesh->GetindexCount(), world, view, proj, currentMesh->GetTextureArray(),
			light->GetDirection(), light->GetDiffuseColor(), XMFLOAT3(0.0f, 0.0f, 1.0f), light->GetSpecularColor(), light->GetSpecularPower());*/
		/*externShaders->RenderTexture(DXEngine->GetDeviceContext(), currentMesh->GetindexCount(), world, view, proj, currentMesh->GetTextureArray()[0]);*/

	}

	for (int i = 0; i < m_ChildMeshs.size(); i++)
	{
		m_ChildMeshs[i]->Render(DXEngine->GetDeviceContext());
	}

	return true;
}

bool ModelPart::Update()
{
	return true;
}

void ModelPart::Quit()
{

}

void ModelPart::InitMesh(FbxNode* node)
{

}

void ModelPart::ReadMaterial(FbxSurfaceMaterial* material)
{

}

void ModelPart::ReadBoneWeight()
{

}