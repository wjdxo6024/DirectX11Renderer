#include "TestAlphaMapObject.h"
#include <fstream>
using namespace std;

TestAlphaMapObject::TestAlphaMapObject()
{

}

TestAlphaMapObject::TestAlphaMapObject(const TestAlphaMapObject& other)
{

}

TestAlphaMapObject::~TestAlphaMapObject()
{

}

bool TestAlphaMapObject::Init(ID3D11Device* device, char* modelfilename, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	m_modelData = new std::vector<ModelVertexForTest>;
	m_modelIndices = new std::vector<unsigned int>;
	m_Counts = new std::pair<int, int>;

	if (!LoadModel(modelfilename))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Load Model.", L"Error", MB_OK);
#endif
		return false;
	}

	// triCount * 3 입니다.
	m_vertexCount = m_Counts->second * 3;
	m_indexCount = m_Counts->second * 3;

	if (!InitializeBuffers(device))
	{
		MessageBox(NULL, L"Cannot Initialize Buffer.", L"Error", MB_OK);
		return false;
	}

	return LoadTexture(device, textureFilename1, textureFilename2);
}

bool TestAlphaMapObject::Init(ID3D11Device* device, char* modelfilename, ID3D11DeviceContext* deviceContext, int args, ...)
{
	m_modelData = new std::vector<ModelVertexForTest>;
	m_modelIndices = new std::vector<unsigned int>;
	m_Counts = new std::pair<int, int>;

	if (!LoadModel(modelfilename))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Load Model.", L"Error", MB_OK);
#endif
		return false;
	}

	// triCount * 3 입니다.
	m_vertexCount = m_Counts->second * 3;
	m_indexCount = m_Counts->second * 3;

	if (!InitializeBuffers(device))
	{
		MessageBox(NULL, L"Cannot Initialize Buffer.", L"Error", MB_OK);
		return false;
	}

	// Load Texture
	va_list fileName;

	va_start(fileName, args);
	
	m_Textures = new TextureArray;
	if (!m_Textures)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create TextureArray.", L"Error", MB_OK);
#endif
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		WCHAR* textureFile = va_arg(fileName, WCHAR*);
		m_Textures->Init(device, textureFile);
	}

	va_end(fileName);
	//--------------------------------------------
	return true;
}

void TestAlphaMapObject::Quit()
{
	ReleaseTexture();

	this->Shutdown();

	ReleaseModel();
}

void TestAlphaMapObject::Render(ID3D11DeviceContext* deviceContext)
{
	// 그리기를 준비하기 위해 그래픽 파이프 라인에 꼭지점과 인덱스 버퍼를 놓는다.
	RenderBuffers(deviceContext);
}

int TestAlphaMapObject::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView** TestAlphaMapObject::GetTextureArray()
{
	return m_Textures->GetTextureArray();
}

bool TestAlphaMapObject::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ModelVertexForTest) * m_modelData->size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = m_modelData->data();
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
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_modelIndices->size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 생성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = m_modelIndices->data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Index Buffer.", L"Error", NULL);
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼 해제
	/*delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
*/
	return true;

}
void TestAlphaMapObject::Shutdown()
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

void TestAlphaMapObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정
	unsigned int stride = sizeof(ModelVertexForTest);
	unsigned int offset = 0;

	// 입력 어셈블러에서 정점 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼를 그릴 기본형을 설정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool TestAlphaMapObject::LoadTexture(ID3D11Device* device, WCHAR* textureFilename1, WCHAR* textureFilename2)
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

void TestAlphaMapObject::ReleaseTexture()
{
	if (m_Textures)
	{
		m_Textures->Quit();
		delete m_Textures;
		m_Textures = 0;
	}
}

bool TestAlphaMapObject::LoadModel(char* filename)
{
	LoadFBX(filename, m_Counts, m_modelData, m_modelIndices);
	return true;
}

void TestAlphaMapObject::ReleaseModel()
{
	if (m_Model)
	{
		delete[] m_Model;
		m_Model = 0;
	}
}