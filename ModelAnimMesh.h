#pragma once
#include "ModelMesh.h"
class ModelAnimMesh :
	public ModelMesh
{
private:
	std::vector<ModelSkinningVertex>* m_ModelData = nullptr;

private:
	void ReadAnim(FbxMesh* mesh);

public:
	ModelAnimMesh();
	ModelAnimMesh(const ModelAnimMesh& other);
	~ModelAnimMesh();

	virtual bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FbxMesh* mesh, bool CpuWrite = false, bool GpuWrite = true);
	virtual bool Render(ID3D11DeviceContext*);
	virtual bool Update();
	virtual bool Quit();
};

