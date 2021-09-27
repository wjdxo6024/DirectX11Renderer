#include "ModelSkeletonBone.h"

ModelSkeletonBone::ModelSkeletonBone(std::string name, int parentBoneIndex)
{

}

ModelSkeletonBone::~ModelSkeletonBone()
{

}

void ModelSkeletonBone::AddAnimationKeyFrames(ModelAnimationKeyFrame* keyFrames)
{

}

void ModelSkeletonBone::SetBindPoseTransform(const XMMATRIX& matBindPostTransform)
{

}

void ModelSkeletonBone::SetBoneReferenceTransform(const XMMATRIX& matBoneReferenceTransform)
{

}

ModelAnimationKeyFrame* ModelSkeletonBone::GetAnimationKeyFrames(std::wstring animationName)
{
	return NULL;
}

const XMMATRIX& ModelSkeletonBone::GetBindPoseTransform()
{
	return XMMATRIX();
}

const XMMATRIX& ModelSkeletonBone::GetInvBindPoseTransform()
{
	return XMMATRIX();
}

const XMMATRIX& ModelSkeletonBone::GetBoneReferenceTransform()
{
	return XMMATRIX();
}

bool ModelSkeletonBone::HaveChild()
{
	if (m_Child.size() > 0)
		return true;
	else
		return false;
}