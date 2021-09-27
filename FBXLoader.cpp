#include "FBXLoader.h"

//TODO : �ؽ��� �����͸� ��µ� ������. �ٸ� ���� �����丵�� �ǽ� �� �ʿ䰡 ����.
// (�ҿ��� ���� uv ��ǥ�� �������� ���� ���̶� �ٸ��� ����..... -> ���� �ؽ��ĸ� 180�� ȸ�� �� ��Ī ��ȯ �ϸ� ��...)
// �� �ִϸ��̼� ������ ���� �� ���� �ǽ� �� ��.

//COMPLETE : �ε��� ������ ���� ���� ��� FBX ������ �ҷ����� ����. �ظ��� �� �� �ҷ���.
// �� ��� ���� ������ ���� ������ ���̴�

// 2020 09 27 ���� ������� ����

void LoadUVInformation(FbxMesh* pMesh, std::vector<ModelBumpVertex>* pOutVertexVector)
{
	//get all UV set names
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					FbxVector2 lUVValue;

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

					//User TODO:
					//Print out the value of UV(lUVValue) or log it to a file
					/*pOutVertexVector->at(lUVIndex)._u = static_cast<float>(lUVValue.mData[0]);
					pOutVertexVector->at(lUVIndex)._v = static_cast<float>(lUVValue.mData[1]);*/
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						//User TODO:
						//Print out the value of UV(lUVValue) or log it to a file
						//pOutVertexVector->at(lPolyIndexCounter)._u = static_cast<float>(lUVValue.mData[0]);
						//pOutVertexVector->at(lPolyIndexCounter)._v = static_cast<float>(lUVValue.mData[1]);
						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}

// For ModelVertex
void LoadUVInformation(FbxMesh* pMesh, std::vector<ModelVertex>* pOutVertexVector)
{
	//get all UV set names
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					FbxVector2 lUVValue;

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

					//User TODO:
					//Print out the value of UV(lUVValue) or log it to a file
					pOutVertexVector->at(lUVIndex)._u = static_cast<float>(lUVValue.mData[0]);
					pOutVertexVector->at(lUVIndex)._v = static_cast<float>(lUVValue.mData[1]);
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						//User TODO:
						//Print out the value of UV(lUVValue) or log it to a file
						pOutVertexVector->at(lPolyIndexCounter)._u = static_cast<float>(lUVValue.mData[0]);
						pOutVertexVector->at(lPolyIndexCounter)._v = static_cast<float>(lUVValue.mData[1]);
						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}

// For TestVertex
void LoadUVInformation(FbxMesh* pMesh, std::vector<ModelVertexForTest>* pOutVertexVector)
{
	//get all UV set names
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					FbxVector2 lUVValue;

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

					//User TODO:
					//Print out the value of UV(lUVValue) or log it to a file
					pOutVertexVector->at(lUVIndex).uv.x = static_cast<float>(lUVValue.mData[0]);
					pOutVertexVector->at(lUVIndex).uv.y = static_cast<float>(lUVValue.mData[1]);
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						//User TODO:
						//Print out the value of UV(lUVValue) or log it to a file
						pOutVertexVector->at(lPolyIndexCounter).uv.x = static_cast<float>(lUVValue.mData[0]);
						pOutVertexVector->at(lPolyIndexCounter).uv.y = static_cast<float>(lUVValue.mData[1]);
						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}


void LoadAnimation(FbxNode* inNode)
{
	FbxSkeleton* fbxSkeleton = inNode->GetSkeleton();
	if (fbxSkeleton == NULL)
		return;

	//if (skeleton != NULL)
	//	skeleton = new ModelSkeleton;
}

// For ModelVertex
void LoadFBX(const char* filename, std::pair<int, int>* memberCounts, std::vector<ModelVertex>* Vertices, std::vector<DWORD>* Indices)
{
	int VertexCount = 0; // ���� ����
	int TriCount = 0; // �ﰢ�� ����

	FbxManager* FbxSdkManager = nullptr;
	if (FbxSdkManager == nullptr)
	{
		FbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(FbxSdkManager, IOSROOT);
		FbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(FbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(FbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(filename, -1, FbxSdkManager->GetIOSettings());

	if (!bSuccess)
	{
		MessageBox(NULL, L"Cannot Open FBX file. Check files.", L"Error", MB_OK);
		return;
	}

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return;

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��.
			FbxGeometryConverter geometryConverter(pFbxChildNode->GetFbxManager());
			geometryConverter.Triangulate(pFbxChildNode->GetNodeAttribute(), true, false);

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();

			VertexCount = pMesh->GetControlPointsCount();
			TriCount = pMesh->GetPolygonCount();

			unsigned int indx = 0;
			int lPolygonCount = pMesh->GetPolygonCount();
			for (int i = 0; i < lPolygonCount; i++)
			{
				int lPolygonsize = pMesh->GetPolygonSize(i);

				for (int pol = 0; pol < lPolygonsize; pol++)
				{
					int index = pMesh->GetPolygonVertex(i, pol);
					Indices->push_back(indx);

					ModelVertex vertex;

					vertex._x = static_cast<float>(pMesh->GetControlPointAt(index).mData[0]);
					vertex._y = static_cast<float>(pMesh->GetControlPointAt(index).mData[1]);
					vertex._z = static_cast<float>(pMesh->GetControlPointAt(index).mData[2]);

					FbxVector4 normalVector;

					pMesh->GetPolygonVertexNormal(i, pol, normalVector);
					/*vertex.normal = D3DXVECTOR3(normalVector.mData[0], normalVector.mData[1], normalVector.mData[2]);*/
					vertex._nx = static_cast<float>(normalVector.mData[0]);
					vertex._ny = static_cast<float>(normalVector.mData[1]);
					vertex._nz = static_cast<float>(normalVector.mData[2]);
					Vertices->push_back(vertex);

					++indx;
				}
			}

			LoadUVInformation(pMesh, Vertices);
		}
	}

	memberCounts->first = VertexCount;
	memberCounts->second = TriCount;

	if (FbxSdkManager != NULL)
	{
		FbxSdkManager->Destroy();
	}
}

void LoadFBXwithBumpMap(const char* filename, std::pair<int, int>* memberCounts, std::vector<ModelBumpVertex>* Vertices, std::vector<unsigned int>* Indices)
{
	int VertexCount = 0; // ���� ����
	int TriCount = 0; // �ﰢ�� ����

	FbxManager* FbxSdkManager = nullptr;
	if (FbxSdkManager == nullptr)
	{
		FbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(FbxSdkManager, IOSROOT);
		FbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(FbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(FbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(filename, -1, FbxSdkManager->GetIOSettings());

	if (!bSuccess)
	{
		MessageBox(NULL, L"Cannot Open FBX file. Check files.", L"Error", MB_OK);
		return;
	}

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return;

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��.
			FbxGeometryConverter geometryConverter(pFbxChildNode->GetFbxManager());
			geometryConverter.Triangulate(pFbxChildNode->GetNodeAttribute(), true, false);

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();

			VertexCount = pMesh->GetControlPointsCount();
			TriCount = pMesh->GetPolygonCount();

			unsigned int indx = 0;
			int lPolygonCount = pMesh->GetPolygonCount();
			for (int i = 0; i < lPolygonCount; i++)
			{
				int lPolygonsize = pMesh->GetPolygonSize(i);

				for (int pol = 0; pol < lPolygonsize; pol++)
				{
					int index = pMesh->GetPolygonVertex(i, pol);
					Indices->push_back(indx);

					ModelBumpVertex vertex;

					// ���ؽ� pos
					/*vertex._x = static_cast<float>(pMesh->GetControlPointAt(index).mData[0]);
					vertex._y = static_cast<float>(pMesh->GetControlPointAt(index).mData[1]);
					vertex._z = static_cast<float>(pMesh->GetControlPointAt(index).mData[2]);*/

					// ��� �Ľ�
					FbxVector4 normalVector;

					pMesh->GetPolygonVertexNormal(i, pol, normalVector);
					/*vertex.normal = D3DXVECTOR3(normalVector.mData[0], normalVector.mData[1], normalVector.mData[2]);*/
					//vertex._nx = static_cast<float>(normalVector.mData[0]);
					//vertex._ny = static_cast<float>(normalVector.mData[1]);
					//vertex._nz = static_cast<float>(normalVector.mData[2]);

					// ź��Ʈ �Ľ�
					int count = pMesh->GetElementTangentCount();
					if (count != 0)
					{
						FbxGeometryElementTangent* tagent = pMesh->GetElementTangent(0);
						int tagentIndex = tagent->GetIndexArray().GetAt(index);
						/*vertex._tx = tagent->GetDirectArray().GetAt(tagentIndex).mData[0];
						vertex._ty = tagent->GetDirectArray().GetAt(tagentIndex).mData[1];
						vertex._tz = tagent->GetDirectArray().GetAt(tagentIndex).mData[2];*/
					}
					else
					{
					/*	vertex._tx = 0;
						vertex._ty = 0;
						vertex._tz = 0;*/
					}

					count = pMesh->GetElementBinormalCount();
					if (count != 0)
					{
						// ���̳�� �Ľ�
						FbxGeometryElementBinormal* binormal = pMesh->GetElementBinormal(0);
						int binormalIndex = binormal->GetIndexArray().GetAt(index);
						/*vertex._bx = binormal->GetDirectArray().GetAt(index).mData[0];
						vertex._by = binormal->GetDirectArray().GetAt(index).mData[1];
						vertex._bz = binormal->GetDirectArray().GetAt(index).mData[2];*/
					}
					else
					{
						//vertex._bx = 0;
						//vertex._by = 0;
						//vertex._bz = 0;
					}

					Vertices->push_back(vertex);

					++indx;
				}
			}

			LoadUVInformation(pMesh, Vertices);
		}
	}

	memberCounts->first = VertexCount;
	memberCounts->second = TriCount;

	if (FbxSdkManager != NULL)
	{
		FbxSdkManager->Destroy();
	}
}

// For TestVertex
void LoadFBX(const char* filename, std::pair<int, int>* memberCounts, std::vector<ModelVertexForTest>* Vertices, std::vector<unsigned int>* Indices)
{
	int VertexCount = 0; // ���� ����
	int TriCount = 0; // �ﰢ�� ����

	FbxManager* FbxSdkManager = nullptr;
	if (FbxSdkManager == nullptr)
	{
		FbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(FbxSdkManager, IOSROOT);
		FbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(FbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(FbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(filename, -1, FbxSdkManager->GetIOSettings());

	if (!bSuccess)
	{
		MessageBox(NULL, L"Cannot Open FBX file. Check files.", L"Error", MB_OK);
		return;
	}

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return;

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��.
			FbxGeometryConverter geometryConverter(pFbxChildNode->GetFbxManager());
			geometryConverter.Triangulate(pFbxChildNode->GetNodeAttribute(), true, false);

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();

			VertexCount = pMesh->GetControlPointsCount();
			TriCount = pMesh->GetPolygonCount();

			unsigned int indx = 0;
			int lPolygonCount = pMesh->GetPolygonCount();
			for (int i = 0; i < lPolygonCount; i++)
			{
				int lPolygonsize = pMesh->GetPolygonSize(i);

				for (int pol = 0; pol < lPolygonsize; pol++)
				{
					int index = pMesh->GetPolygonVertex(i, pol);
					Indices->push_back(indx);

					ModelVertexForTest vertex;

					vertex.pos.x = static_cast<float>(pMesh->GetControlPointAt(index).mData[0]);
					vertex.pos.y = static_cast<float>(pMesh->GetControlPointAt(index).mData[1]);
					vertex.pos.z = static_cast<float>(pMesh->GetControlPointAt(index).mData[2]);

					//FbxVector4 normalVector;

					//pMesh->GetPolygonVertexNormal(i, pol, normalVector);
					///*vertex.normal = D3DXVECTOR3(normalVector.mData[0], normalVector.mData[1], normalVector.mData[2]);*/
					//vertex._nx = static_cast<float>(normalVector.mData[0]);
					//vertex._ny = static_cast<float>(normalVector.mData[1]);
					//vertex._nz = static_cast<float>(normalVector.mData[2]);

					Vertices->push_back(vertex);

					++indx;
				}
			}

			LoadUVInformation(pMesh, Vertices);
		}
	}

	memberCounts->first = VertexCount;
	memberCounts->second = TriCount;

	if (FbxSdkManager != NULL)
	{
		FbxSdkManager->Destroy();
	}
}