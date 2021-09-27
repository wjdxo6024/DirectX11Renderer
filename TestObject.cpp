#include "TestObject.h"

TestObject::TestObject()
{

}

TestObject::TestObject(const TestObject& other)
{

}

TestObject::~TestObject()
{

}

bool TestObject::Init(ID3D11Device* device)
{
	return InitializeBuffers(device);
}

void TestObject::Quit()
{
	this->Shutdown();
}

void TestObject::Render(ID3D11DeviceContext* deviceContext)
{
	// 그리기를 준비하기 위해 그래픽 파이프 라인에 꼭지점과 인덱스 버퍼를 놓는다.
	RenderBuffers(deviceContext);
}

int TestObject::GetIndexCount()
{
	return m_indexCount;
}

bool TestObject::InitializeBuffers(ID3D11Device* device)
{
	// 정점 배열의 정점 수 설정
	m_vertexCount = 3;

	// 인덱스  배열의 인덱스 수 설정
	m_indexCount = 3;

	// 정점 배열 만듬
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		MessageBox(NULL, L"Cannot Create Vertex Array.", L"Error", MB_OK);
		return false;
	}

	// 인덱스 배열
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		MessageBox(NULL, L"Cannot Create Index Array.", L"Error", MB_OK);
		return false;
	}

	// 정점 배열에 데이터 설정
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(1.0f, .0f, 0.0f, 0.5f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f);

	// 인덱스 배열의 값을 설정
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// 정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 정점 버퍼 생성
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Vertex Buffer.", L"Error", MB_OK);
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

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 생성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Index Buffer.", L"Error", NULL);
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼 해제
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;

}
void TestObject::Shutdown()
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

void TestObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// 입력 어셈블러에서 정점 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼를 그릴 기본형을 설정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}