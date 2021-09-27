#include "Sprite.h"

Sprite::Sprite()
{

}

Sprite::Sprite(const Sprite& other)
{

}

Sprite::~Sprite()
{

}

bool Sprite::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, WCHAR* textureFile, int spriteWidth, int spriteHeight)
{
	// ȭ�� ũ�⸦ ��������� ����
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;

	// �������� ��������Ʈ�� �ȼ��� ũ�⸦ ����
	m_spriteWidth = spriteWidth;
	m_spriteHeight = spriteHeight;

	// ���� ������ ��ġ�� ������ �ʱ�ȭ
	m_prePosX = -1;
	m_prePosY = -1;

	// ���� �� �ε��� ���۸� �ʱ�ȭ
	if (!InitBuffers(device))
	{
		MessageBox(NULL, L"Cannot Init Buffer.", L"Sprite Error", MB_OK);
		return false;
	}

	return LoadTexture(device, deviceContext, textureFile);

}

void Sprite::Quit()
{
	ReleaseTexture();

	Shutdown();
}

bool Sprite::Render(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	if (!UpdateBuffers(deviceContext, posX, posY))
	{
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

int Sprite::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Sprite::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Sprite::InitBuffers(ID3D11Device* device)
{
	// ���� �迭�� ���� ���� �ε��� �迭�� �ε��� ���� ����
	m_indexCount = m_vertexCount = 6;

	// ���� �迭�� ����
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// ���� �迭�� 0���� �ʱ�ȭ
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// �ε��� �迭�� ����
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// �����ͷ� �ε��� �迭�� �ε�
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ����
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Vertex Buffer.", L"Sprite Error", MB_OK);
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

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Index Buffer.", L"Sprite Error", MB_OK);
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Sprite::Shutdown()
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

bool Sprite::UpdateBuffers(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// �� ��������Ʈ�� ������ �� ��ġ�� ������� ���� ��� ���� ���۸� ������Ʈ
	// ���� �ù߸� �Ű� ���� ����
	if ((posX == m_prePosX) && (posY == m_prePosY))
	{
		return true;
	}

	// ����� ��� �������Ǵ� ��ġ ������Ʈ
	m_prePosX = posX;
	m_prePosY = posY;

	// ��������Ʈ ������ ȭ�� ��ǥ�� ���
	left = (float)((m_screenWidth / 2) * -1) + (float)posX;

	// ��������Ʈ �������� ȭ�� ��ǥ ���
	right = left + (float)m_spriteWidth;
	
	// ��������Ʈ ����� ȭ�� ��ǥ�� ���
	top = (float)(m_screenHeight / 2) - (float)posY;
	
	// ��������Ʈ �Ʒ����� ȭ�� ��ǥ�� ���
	bottom = top - (float)m_spriteHeight;

	// ���� �迭�� ����
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	if(deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))
	{
		MessageBox(NULL, L"Cannot Lock Memory.", L"Sprite Error", MB_OK);
		return false;
	}

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VertexType*)mappedResource.pData;

	// �����͸� ���� ���ۿ� ����
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// ���� ������ ����� ����
	deviceContext->Unmap(m_vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;
	return true;
	
}

void Sprite::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� ����
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ���� ���۷� �׸� �⺻���� ����, ���⼭�� �ﰢ������ ����
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� ����, ���⼭�� �ﰢ������ ����
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Sprite::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	// �ؽ��� ������Ʈ�� ����
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	// �ؽ��� ������Ʈ�� �ʱ�ȭ
	return m_Texture->Init(device, deviceContext, filename);

}

void Sprite::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Quit();
		delete m_Texture;
		m_Texture = 0;
	}
}