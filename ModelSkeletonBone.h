#pragma once
#include "DXHeader.h"
#include "ModelAnimationKeyFrame.h"
#include <string>
#include <vector>
class ModelSkeletonBone
{
private:
	std::string m_BoneName;
	int m_ParentBoneIndex;

	std::vector<ModelSkeletonBone*> m_Child;

	XMMATRIX LocalSpaceTransformation;
	XMMATRIX WorldSpaceTransformation;
	XMMATRIX BindPoseTransform;
public:
	ModelSkeletonBone(std::string name, int parentBoneIndex);
	~ModelSkeletonBone();

	void AddAnimationKeyFrames(ModelAnimationKeyFrame* keyFrames);

	void SetBindPoseTransform(const XMMATRIX& matBindPostTransform);
	void SetBoneReferenceTransform(const XMMATRIX& matBoneReferenceTransform);

	ModelAnimationKeyFrame* GetAnimationKeyFrames(std::wstring animationName);

	const XMMATRIX& GetBindPoseTransform();
	const XMMATRIX& GetInvBindPoseTransform();
	const XMMATRIX& GetBoneReferenceTransform();

	bool HaveChild();
};


