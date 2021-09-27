#include "ModelMesh.h"

ModelMesh::ModelMesh()
{

}

ModelMesh::ModelMesh(const ModelMesh& other)
{

}

ModelMesh::~ModelMesh()
{

}

bool ModelMesh::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FbxMesh* mesh, bool CpuWrite, bool GpuWrite)
{
	m_ModelData = new std::vector<ModelBumpVertex>;
	m_ModelIndices = new std::vector<DWORD>;

	m_MeshName = mesh->GetName();
	ReadMesh(mesh);

	m_VertexCount = m_ModelData->size();
	m_IndexCount = m_ModelIndices->size();

	m_Device = device;
	m_DeviceContext = deviceContext;

	m_CpuWrite = CpuWrite;
	m_GpuWrite = GpuWrite;

	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(ModelBumpVertex) * m_ModelData->size();
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (m_CpuWrite == false && m_GpuWrite == false)
	{
		BufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // GPU 읽기
	}
	else if (m_CpuWrite == true && m_GpuWrite == false)
	{
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; // GPU 쓰기, GPU 읽기
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (m_CpuWrite == false && m_GpuWrite == true)
	{
		// CPU 쓰기 가능 = UpdateSubResource;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	}
	else
	{
		BufferDesc.Usage = D3D11_USAGE_STAGING;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = m_ModelData->data();
	subResource.SysMemPitch = 0;
	subResource.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&BufferDesc, &subResource, &m_VertexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Vertex Buffer.", L"Error", MB_OK);
		return false;
	}

	// Index Buffer
	// 정적 인덱스 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * m_ModelIndices->size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 생성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = m_ModelIndices->data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Index Buffer.", L"Error", NULL);
		return false;
	}

	// 컨스턴트 버퍼 

	D3D11_BUFFER_DESC ConstantBufferDesc;
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBufferDesc.ByteWidth = sizeof(BumpConstantBuffer);
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstantBufferDesc.MiscFlags = 0;
	ConstantBufferDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&ConstantBufferDesc, NULL, &m_ConstantBuffer)))
	{
		MessageBox(NULL, L"Cannot Create Constant Buffer.", L"Error", NULL);
		return false;
	}

	// Read Texture
	ReadTexture();
}

bool ModelMesh::Render(ID3D11DeviceContext* deviceContext)
{
	UINT offset = 0;
	UINT stride = sizeof(ModelBumpVertex);

	m_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride,  &offset);
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	// Constant
	//if (m_ConstantBuffer != NULL)
	//{
	//	D3D11_MAPPED_SUBRESOURCE subResource;
	//	m_DeviceContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	//	{
	//		memcpy(subResource.pData, m_ConstantData, m_ConstantDataSize);
	//	}

	//	m_DeviceContext->Unmap(m_ConstantBuffer, 0);
	//	//---------------------------
	//}

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return true;
}

bool ModelMesh::Update()
{
	return true;
}

bool ModelMesh::Quit()
{
	if (m_Textures)
	{
		m_Textures->Quit();
		delete m_Textures;
	}

	if (m_ModelData)
	{
		delete m_ModelData;
	}

	if (m_ModelIndices)
	{
		delete m_ModelIndices;
	}

	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		delete m_IndexBuffer;
		m_IndexBuffer = 0;
	}

	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		delete m_VertexBuffer;
		m_VertexBuffer = 0;
	}

	return true;
}

// Private
void ModelMesh::ReadMesh(FbxMesh* mesh)
{
	unsigned int indx = 0; // = VertexCounter
	int lPolygonCount = mesh->GetPolygonCount();

	for (int i = 0; i < lPolygonCount; i++)
	{
		int lPolygonsize = mesh->GetPolygonSize(i);

		for (int pol = 0; pol < lPolygonsize; pol++)
		{
			int index = mesh->GetPolygonVertex(i, pol);
			m_ModelIndices->push_back(indx);

			ModelBumpVertex vertex;

			// 버텍스 pos
			vertex.pos.x = static_cast<float>(mesh->GetControlPointAt(index).mData[0]);
			vertex.pos.y = static_cast<float>(mesh->GetControlPointAt(index).mData[1]);
			vertex.pos.z = static_cast<float>(mesh->GetControlPointAt(index).mData[2]);

			// 노멀 파싱
			FbxVector4 normalVector;

			mesh->GetPolygonVertexNormal(i, pol, normalVector);
			/*vertex.normal = D3DXVECTOR3(normalVector.mData[0], normalVector.mData[1], normalVector.mData[2]);*/
			vertex.normal.x = static_cast<float>(normalVector.mData[0]);
			vertex.normal.y = static_cast<float>(normalVector.mData[1]);
			vertex.normal.z = static_cast<float>(normalVector.mData[2]);


			XMFLOAT3 Tanget;
			ReadTangent(mesh, index, indx, Tanget);
			vertex.tangent.x = Tanget.x;
			vertex.tangent.y = Tanget.y;
			vertex.tangent.z = Tanget.z;


			XMFLOAT3 binormal;
			ReadBinormal(mesh, index, indx, binormal);
			vertex.binormal.x = binormal.x;
			vertex.binormal.y = binormal.y;
			vertex.binormal.z = binormal.z;


			XMFLOAT2 uv;

			ReadUV(mesh, index, mesh->GetTextureUVIndex(i, pol), 0, uv);
			vertex.uv.x = uv.x;
			vertex.uv.y = uv.y;

			m_ModelData->push_back(vertex);
			
			++indx;
		}
	}

}

void ModelMesh::ReadVertex(FbxMesh* mesh)
{

}

void ModelMesh::ReadNormal(FbxMesh* mesh, int ControlPoint, int VertexCount, XMFLOAT3& outNormal)
{

}

void ModelMesh::ReadTangent(FbxMesh* mesh, int ControlPoint, int VertexCount, XMFLOAT3& outTangent)
{
	if (mesh->GetElementTangentCount() < 1)
	{
		outTangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return;
	}

	FbxGeometryElementTangent* Tangent = mesh->GetElementTangent(0);

	switch (Tangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (Tangent->GetReferenceMode())
		{
			case FbxGeometryElement::eDirect:
			{
				outTangent.x = static_cast<float>(Tangent->GetDirectArray().GetAt(ControlPoint).mData[0]);
				outTangent.y = static_cast<float>(Tangent->GetDirectArray().GetAt(ControlPoint).mData[1]);
				outTangent.z = static_cast<float>(Tangent->GetDirectArray().GetAt(ControlPoint).mData[2]);
			}
			break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int index = Tangent->GetIndexArray().GetAt(ControlPoint);
				outTangent.x = static_cast<float>(Tangent->GetDirectArray().GetAt(index).mData[0]);
				outTangent.y = static_cast<float>(Tangent->GetDirectArray().GetAt(index).mData[1]);
				outTangent.z = static_cast<float>(Tangent->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			default:
			{
				outTangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
			}
			break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (Tangent->GetReferenceMode())
		{
			case FbxGeometryElement::eDirect:
			{
				outTangent.x = static_cast<float>(Tangent->GetDirectArray().GetAt(VertexCount).mData[0]);
				outTangent.y = static_cast<float>(Tangent->GetDirectArray().GetAt(VertexCount).mData[1]);
				outTangent.z = static_cast<float>(Tangent->GetDirectArray().GetAt(VertexCount).mData[2]);
			}
			break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int index = Tangent->GetIndexArray().GetAt(VertexCount);
				outTangent.x = static_cast<float>(Tangent->GetDirectArray().GetAt(index).mData[0]);
				outTangent.y = static_cast<float>(Tangent->GetDirectArray().GetAt(index).mData[1]);
				outTangent.z = static_cast<float>(Tangent->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			default:
			{
				outTangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
			}
			break;
		}
		break;
	}
}

void ModelMesh::ReadBinormal(FbxMesh* mesh, int ControlPoint, int VertexCount, XMFLOAT3& outBinormal)
{
	if (mesh->GetElementBinormalCount() < 1)
	{
		outBinormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return;
	}

	FbxGeometryElementBinormal* Binormal = mesh->GetElementBinormal(0);

	switch (Binormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (Binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outBinormal.x = static_cast<float>(Binormal->GetDirectArray().GetAt(ControlPoint).mData[0]);
			outBinormal.y = static_cast<float>(Binormal->GetDirectArray().GetAt(ControlPoint).mData[1]);
			outBinormal.z = static_cast<float>(Binormal->GetDirectArray().GetAt(ControlPoint).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = Binormal->GetIndexArray().GetAt(ControlPoint);
			outBinormal.x = static_cast<float>(Binormal->GetDirectArray().GetAt(index).mData[0]);
			outBinormal.y = static_cast<float>(Binormal->GetDirectArray().GetAt(index).mData[1]);
			outBinormal.z = static_cast<float>(Binormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
		{
			outBinormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
		break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (Binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outBinormal.x = static_cast<float>(Binormal->GetDirectArray().GetAt(VertexCount).mData[0]);
			outBinormal.y = static_cast<float>(Binormal->GetDirectArray().GetAt(VertexCount).mData[1]);
			outBinormal.z = static_cast<float>(Binormal->GetDirectArray().GetAt(VertexCount).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = Binormal->GetIndexArray().GetAt(VertexCount);
			outBinormal.x = static_cast<float>(Binormal->GetDirectArray().GetAt(index).mData[0]);
			outBinormal.y = static_cast<float>(Binormal->GetDirectArray().GetAt(index).mData[1]);
			outBinormal.z = static_cast<float>(Binormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
		{
			outBinormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
		break;
		}
		break;
	}
}

void ModelMesh::ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, XMFLOAT2& outUV)
{
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
	{
		throw std::exception("Invalid UV Layer Number");
	}
	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

int ModelMesh::GetindexCount()
{
	return m_IndexCount;
}

void ModelMesh::ReadTexture()
{
	m_Textures = new TextureArray;
	if (!m_Textures)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create TextureArray.", L"Error", MB_OK);
#endif
		return;
	}

	// TODO : 나중에 바꿀것
	//std::string textureFile = "./Texture/" + m_MeshName + "_Texture.dds";
	std::wstring TextureFile = L"./Texture/Dragon_Bump_Col2.dds";
	std::wstring BumpFile = L"./Texture/Dragon_Nor_mirror2.dds";
	std::wstring SpecFile = L"./Texture/spec02.dds";

	/*m_Textures->Init(m_Device, 2, TextureFile.c_str(), BumpFile.c_str());*/
	m_Textures->Init(m_Device, 2, TextureFile.c_str(), BumpFile.c_str());
}

ID3D11ShaderResourceView** ModelMesh::GetTextureArray()
{
	return m_Textures->GetTextureArray();
}