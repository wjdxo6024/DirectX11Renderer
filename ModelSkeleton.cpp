#include "ModelSkeleton.h"

ModelSkeleton::ModelSkeleton()
{

}

ModelSkeleton::ModelSkeleton(const ModelSkeleton& other)
{

}

ModelSkeleton::~ModelSkeleton()
{

}

bool ModelSkeleton::Init(FbxScene* fbxScene)
{
	if (fbxScene->GetSrcObjectCount<FbxSkeleton>() > 0) // ½ºÄÌ·¹ÅæÀÌ Á¸ÀçÇÑ´Ù.
	{
		FbxNode* root = fbxScene->GetRootNode();
		
	}

	return true;
}

void ModelSkeleton::ReadBone(FbxNode* node, int parentBoneIndex)
{

}

void ModelSkeleton::AddSkeletonBone(ModelSkeletonBone* skeletonBone)
{

}

ModelSkeletonBone* ModelSkeleton::FindBone(std::string boneName)
{
	return NULL;
}

const std::vector<ModelSkeletonBone*> & ModelSkeleton::GetSkeletonBones()
{
	return m_SkeletonBones;
}

XMMATRIX* ModelSkeleton::GetSkinTransforms()
{
	return m_SkinTransforms;
}

int ModelSkeleton::GetBoneCount()
{
	return m_BoneCount;
}

void ModelSkeleton::UpdateAnimation(ModelAnimationController* animationController)
{

}

void ModelSkeleton::BuildBoneTransforms(ModelAnimationController* animationController, std::wstring animationName)
{

}