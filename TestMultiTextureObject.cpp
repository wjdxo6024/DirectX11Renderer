#include "TestMultiTextureObject.h"
#include <fstream>
using namespace std;

TestMultiTextureObject::TestMultiTextureObject()
{

}

TestMultiTextureObject::TestMultiTextureObject(const TestMultiTextureObject& other)
{

}

TestMultiTextureObject::~TestMultiTextureObject()
{

}

bool TestMultiTextureObject::Init(ID3D11Device* device, char* modelfilename, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	m_modelData = new std::vector<ModelVertex>;
	m_modelIndices = new std::vector<DWORD>;
	m_Counts = new std::pair<int, int>;

	if (!LoadModel(modelfilename))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Load Model.", L"Error", MB_OK);
#endif
		return false;
	}

	// triCount * 3 �Դϴ�.
	m_vertexCount = m_Counts->second * 3;
	m_indexCount = m_Counts->second * 3;

	if (!InitializeBuffers(device))
	{
		MessageBox(NULL, L"Cannot Initialize Buffer.", L"Error", MB_OK);
		return false;
	}

	return LoadTexture(device, textureFilename1, textureFilename2);
}

void TestMultiTextureObject::Quit()
{
	ReleaseTexture();

	this->Shutdown();

	ReleaseModel();
}

void TestMultiTextureObject::Render(ID3D11DeviceContext* deviceContext)
{
	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� ���´�.
	RenderBuffers(deviceContext);
}

int TestMultiTextureObject::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView** TestMultiTextureObject::GetTextureArray()
{
	return m_Textures->GetTextureArray();
}

bool TestMultiTextureObject::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_modelData->at(i)._x, m_modelData->at(i)._y, m_modelData->at(i)._z);
		vertices[i].texture = XMFLOAT2(m_modelData->at(i)._u, m_modelData->at(i)._v);

	}

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = m_modelIndices->at(i);
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

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
void TestMultiTextureObject::Shutdown()
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

void TestMultiTextureObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool TestMultiTextureObject::LoadTexture(ID3D11Device* device, WCHAR* textureFilename1, WCHAR* textureFilename2)
{

	m_Textures = new TextureArray;
	if (!m_Textures)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create TextureArray.", L"Error", MB_OK);
#endif
		return false;
	}

	return m_Textures->Init(device, textureFilename1, textureFilename2);
}

void TestMultiTextureObject::ReleaseTexture()
{
	if (m_Textures)
	{
		m_Textures->Quit();
		delete m_Textures;
		m_Textures = 0;
	}
}

bool TestMultiTextureObject::LoadModel(char* filename)
{
	LoadFBX(filename, m_Counts, m_modelData, m_modelIndices);
	return true;
}

void TestMultiTextureObject::ReleaseModel()
{
	if (m_Model)
	{
		delete[] m_Model;
		m_Model = 0;
	}
}