#include "TestTextureObject.h"

TestTextureObject::TestTextureObject()
{

}

TestTextureObject::TestTextureObject(const TestTextureObject& other)
{

}

TestTextureObject::~TestTextureObject()
{

}

bool TestTextureObject::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename)
{
	if (!InitializeBuffers(device))
	{
		MessageBox(NULL, L"Cannot  Initialize Buffer.", L"Error", MB_OK);
		return false;
	}

	return LoadTexture(device, deviceContext, textureFilename);
}

void TestTextureObject::Quit()
{
	ReleaseTexture();

	this->Shutdown();
}

void TestTextureObject::Render(ID3D11DeviceContext* deviceContext)
{
	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� ���´�.
	RenderBuffers(deviceContext);
}

int TestTextureObject::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* TestTextureObject::GetTexture()
{
	return m_Texture->GetTexture();
}

bool TestTextureObject::InitializeBuffers(ID3D11Device* device)
{
	// ���� �迭�� ���� �� ����
	m_vertexCount = 3;

	// �ε���  �迭�� �ε��� �� ����
	m_indexCount = 3;

	// ���� �迭 ����
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		MessageBox(NULL, L"Cannot Create Vertex Array.", L"Error", MB_OK);
		return false;
	}

	// �ε��� �迭
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		MessageBox(NULL, L"Cannot Create Index Array.", L"Error", MB_OK);
		return false;
	}

	// ���� �迭�� ������ ����
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); // Bottom left
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f); // Top middle
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f); // Bottom right
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	// �ε��� �迭�� ���� ����
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// ���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� ������ ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ����
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Vertex Buffer.", L"Error", MB_OK);
		return false;
	}

	// ���� �ε��� ������ ����ü�� ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� ����
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Index Buffer.", L"Error", NULL);
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���� ����
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;

}
void TestTextureObject::Shutdown()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void TestTextureObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� ����
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// �Է� ��������� ���� ���۸� Ȱ������ ����
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// �Է� ��������� �ε��� ���۸� Ȱ������ ����
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۸� �׸� �⺻���� ����
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool TestTextureObject::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	return m_Texture->Init(device, deviceContext, filename);
}

void TestTextureObject::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Quit();
		delete m_Texture;
		m_Texture = 0;
	}
}