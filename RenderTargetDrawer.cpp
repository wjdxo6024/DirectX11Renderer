#include "RenderTargetDrawer.h"

RenderTargetDrawer::RenderTargetDrawer()
{

}

RenderTargetDrawer::RenderTargetDrawer(const RenderTargetDrawer& other)
{

}

RenderTargetDrawer::~RenderTargetDrawer()
{

}

bool RenderTargetDrawer::Init(ID3D11Device* device, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapHeight = bitmapHeight;
	m_bitmapWidth = bitmapWidth;

	m_previousPosX = -1;
	m_previousPosY = -1;

	return InitBuffers(device);
}

void RenderTargetDrawer::Quit()
{
	Shutdown();
}

bool RenderTargetDrawer::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	if (!UpdateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

int RenderTargetDrawer::GetIndexCount()
{
	return m_indexCount;
}

bool RenderTargetDrawer::InitBuffers(ID3D11Device* device)
{
	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Vertices.", L"RTT Error", MB_OK);
#endif
		return false;
	}

	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Indices.", L"RTT Error", MB_OK);
#endif
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Vertex Buffer.", L"RTT Error", MB_OK);
#endif
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
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Index Buffer.", L"RTT Error", MB_OK);
#endif
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void RenderTargetDrawer::Shutdown()
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

bool RenderTargetDrawer::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	m_previousPosX = positionX;
	m_previousPosY = positionY;

	float left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	float right = left + (float)m_bitmapWidth;
	float top = (float)(m_screenHeight / 2) - (float)positionY;
	float bottom = top - (float)m_bitmapHeight;

	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Vertex Memory.", L"RTT Error", MB_OK);
#endif
		return false;
	}

	// first triangle
	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// second triangle
	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Map VertexBuffer.", L"RTT Drawer Error", MB_OK);
#endif
		return false;
	}

	VertexType* verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)* m_vertexCount));
	
	deviceContext->Unmap(m_vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

void RenderTargetDrawer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}