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
	// 화면 크기를 멤버변수에 저장
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;

	// 렌더링할 스프라이트의 픽셀의 크기를 저장
	m_spriteWidth = spriteWidth;
	m_spriteHeight = spriteHeight;

	// 이전 렌더링 위치를 음수로 초기화
	m_prePosX = -1;
	m_prePosY = -1;

	// 정점 및 인덱스 버퍼를 초기화
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
	// 정점 배열의 정점 수와 인덱스 배열의 인덱스 수를 지정
	m_indexCount = m_vertexCount = 6;

	// 정점 배열을 만듬
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 정점 배열을 0으로 초기화
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// 인덱스 배열을 만듬
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 데이터로 인덱스 배열을 로드
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 정점 버퍼 생성
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Vertex Buffer.", L"Sprite Error", MB_OK);
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성
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

	// 이 스프라이트를 렌더링 할 위치가 변경되지 않은 경우 정점 버퍼를 업데이트
	// 현재 올발른 매개 변수 존재
	if ((posX == m_prePosX) && (posY == m_prePosY))
	{
		return true;
	}

	// 변경된 경우 렌더링되는 위치 업데이트
	m_prePosX = posX;
	m_prePosY = posY;

	// 스프라이트 왼쪽의 화면 좌표를 계산
	left = (float)((m_screenWidth / 2) * -1) + (float)posX;

	// 스프라이트 오른쪽의 화면 좌표 계산
	right = left + (float)m_spriteWidth;
	
	// 스프라이트 상단의 화면 좌표를 계산
	top = (float)(m_screenHeight / 2) - (float)posY;
	
	// 스프라이트 아래쪽의 화면 좌표를 계산
	bottom = top - (float)m_spriteHeight;

	// 정점 배열을 만듬
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

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	verticesPtr = (VertexType*)mappedResource.pData;

	// 데이터를 정점 버퍼에 복사
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// 정점 버퍼의 잠금을 해제
	deviceContext->Unmap(m_vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;
	return true;
	
}

void Sprite::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 정점 버퍼로 그릴 기본형을 설정, 여기서는 삼각혁으로 설정
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정, 여기서는 삼각형으로 설정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Sprite::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	// 텍스쳐 오브젝트를 생성
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	// 텍스쳐 오브젝트를 초기화
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