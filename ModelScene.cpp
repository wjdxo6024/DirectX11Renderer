#include "ModelScene.h"

ModelScene::ModelScene()
{

}

ModelScene::ModelScene(const ModelScene& other)
{

}

ModelScene::~ModelScene()
{

}

bool ModelScene::Init(DirectXEngine* dxEngine, Camera* camera, ModelShaders* shaders, const char* filePath)
{
	externEngine = dxEngine;
	externCamera = camera;
	externShaders = shaders;

	m_FbxManager = FbxManager::Create();
	
	m_FbxScene = FbxScene::Create(m_FbxManager, "");

	m_FbxConverter = new FbxGeometryConverter(m_FbxManager);
	{
		if (filePath != NULL)
		{
			if (!LoadScene(filePath))
				return false;
		}

	}

	if (m_FbxConverter != NULL)
	{
		m_FbxConverter->~FbxGeometryConverter();
		delete m_FbxConverter;
		m_FbxConverter = NULL;
	}

	return true;
}

bool ModelScene::Render(XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj)
{
	m_Model->Render(world, view, proj, externLights[0]);
	return true;
}

bool ModelScene::Update()
{
	return true;
}

bool ModelScene::LoadScene(const char* filePath)
{
	std::string File = filePath;

	// SDK Version
	int sdkMajor, sdkMinor, sdkRevision;
	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

	m_FbxImporter = FbxImporter::Create(m_FbxManager, "");
	bool bSuccess = m_FbxImporter->Initialize(File.c_str(), -1, m_FbxManager->GetIOSettings());

	FbxScene* pFbxScene = FbxScene::Create(m_FbxManager, "");

	bSuccess = m_FbxImporter->Initialize(filePath, -1, m_FbxManager->GetIOSettings());

	if (!bSuccess)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Open FBX file. Check files.", L"Error", MB_OK);
#endif
		return false;
	}
	
	bSuccess = m_FbxImporter->Import(m_FbxScene);

	// 좌표축을 재설정
	FbxAxisSystem SceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	if (SceneAxisSystem != OurAxisSystem)
	{
		OurAxisSystem.ConvertScene(pFbxScene);
	}

	// 모델 생성
	m_Model = new Model;
	if (!m_Model->init(externEngine, m_FbxScene->GetRootNode()))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Model. Check files.", L"Error", MB_OK);
#endif
		return false;
	}

	// 씬을 분류하면서 생성
	ProcessScene();

	m_FbxImporter->Destroy();
	return true;
}

bool ModelScene::ProcessScene()
{
	m_Skeleton = NULL;
	m_AnimationController = NULL;

	bool HaveMaterial = false;
	bool HaveMesh = false;
	bool HaveSkeleton = false;
	bool HaveAnimation = false;

	// 우선 Animation이 존재하는 지 확인
	int animationCount = m_FbxScene->GetSrcObjectCount<FbxAnimStack>();
	int materialCount = m_FbxScene->GetMaterialCount();

	int GeometryCount = m_FbxScene->GetGeometryCount();
	int skeletonCount = m_FbxScene->GetSrcObjectCount<FbxSkeleton>();

	if(skeletonCount)
		LoadSkeleton(m_FbxScene->GetRootNode(), FbxNodeAttribute::eSkeleton);
	
	if (GeometryCount)
	{
		if (animationCount)
		{
			// TODO : 잠시 출력만을 위하여 false로 사용 나중에 true로 애니메이션 스키닝으로 전환 할 것
			LoadMesh(m_FbxScene->GetRootNode(), FbxNodeAttribute::eMesh, false);
		}
		else
			LoadMesh(m_FbxScene->GetRootNode(), FbxNodeAttribute::eMesh, false);
	}
	
	if (animationCount)
		LoadAnimation();

	return true;
}

void ModelScene::Quit()
{
	if (m_Model != NULL)
	{
		m_Model->Quit();
		delete m_Model;
		m_Model = 0;
	}
}

// Private
bool ModelScene::LoadAnimation()
{
	return true;
}

bool ModelScene::LoadMesh(FbxNode* root, FbxNodeAttribute::EType attribute, bool IsAnimated)
{
	FbxNodeAttribute* NodeAttribute = root->GetNodeAttribute();
	m_FbxConverter->Triangulate(NodeAttribute, true, false);

	int childCount = root->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* pFbxChildNode = root->GetChild(i);

		if (pFbxChildNode->GetNodeAttribute() == NULL)
			continue;

		FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
		if (AttributeType == FbxNodeAttribute::eMesh)
		{
			ModelPart* mesh;
			mesh = new ModelPart;
			// ModelPart를 생성하고 m_Model에 집어 넣는다.
			if (IsAnimated)
			{
				mesh->Init(externEngine, pFbxChildNode, IsAnimated, externShaders);
			}
			else
			{
				mesh->Init(externEngine, pFbxChildNode, IsAnimated, externShaders);
			}

			m_Model->PushModelPart(mesh);
		}
	}

	// std::vector<ModelBoneWeight> boneWeights(vertexCount, ModelBoneWeight()):
	// LoadBoneWeight();

	return true;
}

bool ModelScene::LoadSkeleton(FbxNode* root, FbxNodeAttribute::EType attribute)
{

	return true;
}

bool ModelScene::LoadBoneWeights(FbxMesh* mesh, std::vector<ModelBoneWeight>& meshBoneWeight)
{
	if (m_Skeleton == NULL)
		return false;

	for (int i = 0; i < mesh->GetDeformerCount(); i++)
	{
		FbxDeformer* deformer = mesh->GetDeformer(i);

		if (deformer == NULL)
			continue;

		// Skinned Mesh
		if (deformer->GetDeformerType() == FbxDeformer::eSkin)
		{
			FbxSkin* skin = (FbxSkin *)deformer;

			if (skin == NULL)
				continue;

			LoadBoneWeightSkin(skin, meshBoneWeight);
		}
	}
	return true;
}

bool ModelScene::LoadBoneWeightSkin(FbxSkin* skin, std::vector<ModelBoneWeight>& meshBoneWeights)
{
	FbxCluster::ELinkMode linkNode = FbxCluster::eNormalize;

	int clusterCount = skin->GetClusterCount();
	std::vector<ModelBoneWeight> skinBoneWeights(meshBoneWeights.size(), ModelBoneWeight());

	for (int i = 0; i < clusterCount; i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);
		if (cluster == NULL)
			continue;

		linkNode = cluster->GetLinkMode();
		FbxNode* pLinkNode = cluster->GetLink(); // Joint

		if (pLinkNode == NULL)
			continue;

		std::wstring lineNodeName = StringToWstring(pLinkNode->GetName());
		
	}
	return true;
}

void ModelScene::PushLight(Light* light)
{
	externLights.push_back(light);
}