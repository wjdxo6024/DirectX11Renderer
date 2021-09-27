#pragma once
#include "DXHeader.h"
#include "TextureArray.h"
#include <fbxsdk.h>
#include <vector>

struct BumpConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};

class ModelMesh
{
private:
	std::vector<ModelBumpVertex>* m_ModelData = nullptr;

protected:
	std::string m_MeshName;

	std::vector<DWORD>* m_ModelIndices;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	UINT m_VertexCount;

	ID3D11Buffer* m_IndexBuffer = nullptr;
	UINT* index;
	UINT m_IndexCount;

	ID3D11Buffer* m_ConstantBuffer = nullptr; // Shader
	VOID* m_ConstantData;
	UINT m_ConstantDataSize;

	bool m_CpuWrite;
	bool m_GpuWrite;

	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;

	UINT m_Slot;
	UINT m_Offset;
	UINT m_Stride;

	TextureArray* m_Textures = nullptr;

protected:
	void ReadMesh(FbxMesh* mesh);

	// TODO : 나중에 분리 시킬 것
	void ReadVertex(FbxMesh* mesh);
	void ReadNormal(FbxMesh* mesh, int ControlPoint, int VertexCount, XMFLOAT3& outNormal);
	void ReadTangent(FbxMesh* mesh, int ControlPoint, int VertexCount, XMFLOAT3& outTangent);
	void ReadBinormal(FbxMesh* mesh, int ControlPoint, int VertexCount, XMFLOAT3& outBinormal);

	void ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, XMFLOAT2& outUV);

	void ReadTexture();
public:
	ModelMesh();
	ModelMesh(const ModelMesh& other);
	~ModelMesh();

	virtual bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FbxMesh* mesh, bool CpuWrite = false, bool GpuWrite = true);
	virtual bool Render(ID3D11DeviceContext*);
	virtual bool Update();
	virtual bool Quit();

	int GetindexCount();
	ID3D11ShaderResourceView** GetTextureArray();
};
