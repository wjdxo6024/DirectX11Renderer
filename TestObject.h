#pragma once
#include "DXHeader.h"
#include "Object.h"
#include "Texture.h"

class TestObject :
	public Object, public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;

public:
	TestObject();
	TestObject(const TestObject&);
	~TestObject();

	bool Init(ID3D11Device*);
	void Quit();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void Shutdown();
	void RenderBuffers(ID3D11DeviceContext*);

};

