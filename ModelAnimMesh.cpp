#include "ModelAnimMesh.h"

ModelAnimMesh::ModelAnimMesh()
{

}

ModelAnimMesh::ModelAnimMesh(const ModelAnimMesh& other)
{

}

ModelAnimMesh::~ModelAnimMesh()
{

}

bool ModelAnimMesh::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FbxMesh* mesh, bool CpuWrite, bool GpuWrite)
{
	return true;
}

bool ModelAnimMesh::Render(ID3D11DeviceContext* deviceContext)
{
	return true;
}

bool ModelAnimMesh::Update()
{
	return true;
}

bool ModelAnimMesh::Quit()
{
	return true;
}

// Private

void ModelAnimMesh::ReadAnim(FbxMesh* mesh)
{

}