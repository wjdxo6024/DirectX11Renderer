#pragma once
#include "DXHeader.h"
#include "ModelSkeletonBone.h"
#include "ModelAnimationController.h"
#include <string>
#include <vector>
#include <fbxsdk.h>

class ModelSkeleton
{
private:
	std::vector<ModelSkeletonBone*> m_SkeletonBones;
	XMMATRIX* m_SkinTransforms;
	XMMATRIX* m_BoneAnimationTransforms;

	int m_BoneCount;
private:
	void BuildBoneTransforms(ModelAnimationController* animationController, std::wstring animationName);
	void ReadBone(FbxNode* node, int parentBoneIndex);
public:
	ModelSkeleton();
	ModelSkeleton(const ModelSkeleton& other);
	~ModelSkeleton();

	bool Init(FbxScene* fbxScene);
	void Render(); // Does not Use
	void Update();
	void Quit();

	void AddSkeletonBone(ModelSkeletonBone* skeletonBone);
	ModelSkeletonBone* FindBone(std::string boneName);
	const std::vector<ModelSkeletonBone*> & GetSkeletonBones();

	XMMATRIX* GetSkinTransforms();
	int GetBoneCount();
	void UpdateAnimation(ModelAnimationController* animationController);
	
};

