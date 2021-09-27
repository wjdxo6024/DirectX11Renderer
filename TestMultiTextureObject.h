#pragma once
#include "DXHeader.h"
#include "Object.h"
#include "TextureArray.h"
#include "FBXLoader.h"

class TestMultiTextureObject :
	public Object, public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	TextureArray* m_Textures = nullptr;
	ModelType* m_Model = nullptr;

	std::vector<ModelVertex>* m_modelData = nullptr;
	std::vector<DWORD>* m_modelIndices = nullptr;
	std::pair<int, int>* m_Counts;
public:
	TestMultiTextureObject();
	TestMultiTextureObject(const TestMultiTextureObject&);
	~TestMultiTextureObject();

	bool Init(ID3D11Device* device, char* modelfilename, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename1, WCHAR* textureFilename2);
	void Quit();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTextureArray();
private:
	bool InitializeBuffers(ID3D11Device*);
	void Shutdown();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
};
