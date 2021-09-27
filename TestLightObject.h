#pragma once
#include "DXHeader.h"
#include "Object.h"
#include "Texture.h"

class TestLightObject : public Object, public AlignedAllocationPolicy<16>
{
private:
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

public:
	TestLightObject();
	TestLightObject(const TestLightObject&);
	~TestLightObject();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);
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
};

