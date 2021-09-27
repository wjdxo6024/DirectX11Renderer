#include "ModelObject.h"

ModelObject::ModelObject()
{

}

ModelObject::ModelObject(const ModelObject& other)
{

}

ModelObject::~ModelObject()
{

}

bool ModelObject::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, char* modelName)
{
	m_modelData = new std::vector<ModelVertex>;
	m_modelIndices = new std::vector<DWORD>;
	m_Counts = new std::pair<int, int>;

	if (!LoadModel(modelName))
	{
		MessageBox(NULL, L"Cannot Load Model.", L"Error", MB_OK);
		return false;
	}

	// triCount * 3 ÀÔ´Ï´Ù.
	m_vertexCount =  m_Counts->second * 3;
	m_indexCount = m_Counts->second * 3;

	if (!InitializeBuffers(device))
	{
		MessageBox(NULL, L"Cannot Init Buffers.", L"Error", MB_OK);
		return false;
	}
	return LoadTexture(device, deviceContext, textureFilename);
}

void ModelObject::Quit()
{
	Shutdown();

	ReleaseTexture();

	ReleaseModel();
}

void ModelObject::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelObject::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelObject::GetTexture()
{
	return m_Texture->GetTexture();
}
bool ModelObject::InitializeBuffers(ID3D11Device* device)
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
		vertices[i].normal = XMFLOAT3(m_modelData->at(i)._nx, m_modelData->at(i)._ny, m_modelData->at(i)._nz);
	
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

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Vertex Buffer.", L"Error", MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Index Buffer.", L"Error", MB_OK);
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelObject::Shutdown()
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

void ModelObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelObject::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	return m_Texture->Init(device, deviceContext, filename);

	return true;
}

void ModelObject::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Quit();
		delete m_Texture;
		m_Texture = 0;
	}
}

bool ModelObject::LoadModel(char* filename)
{
	LoadFBX(filename, m_Counts, m_modelData, m_modelIndices);
	return true;
}

void ModelObject::ReleaseModel()
{
	if (!m_modelData)
	{
		m_modelData->clear();
		delete m_modelData;
		m_modelData = 0;
	}
	if (!m_modelIndices)
	{
		m_modelIndices->clear();
		delete m_modelIndices;
		m_modelIndices = 0;
	}
	if (!m_Counts)
	{
		delete m_Counts;
		m_Counts = 0;
	}
}