#pragma once
#include "Object.h"
#include "Texture.h"
#include "DXHeader.h"
#include "FBXLoader.h"
#include <vector>

class ModelObject :
	public Object, public AlignedAllocationPolicy<16>
{
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	Texture* m_Texture = nullptr;
	ModelVertex* m_model = nullptr;

	std::vector<ModelVertex>* m_modelData = nullptr;
	std::vector<DWORD>* m_modelIndices = nullptr;
	std::pair<int, int>* m_Counts;
public:
	ModelObject();
	ModelObject(const ModelObject&);
	~ModelObject();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, char* modelName);
	void Quit();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitializeBuffers(ID3D11Device*);
	void Shutdown();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
};

