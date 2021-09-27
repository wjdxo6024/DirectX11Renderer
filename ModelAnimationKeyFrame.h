#pragma once
#include "ModelAnimationQuaternionKeyFrame.h"
#include "DXHeader.h"
#include <vector>
#include <string>
class ModelAnimationKeyFrame
{
private:
	std::string animationName;

	std::vector < std::pair<XMMATRIX, ModelAnimationQuaternionKeyFrame>> keyFrames;

public:
	ModelAnimationKeyFrame();
	ModelAnimationKeyFrame(const ModelAnimationKeyFrame& other);
	~ModelAnimationKeyFrame();

	bool Init(std::string animationName);
	bool AddKeyFrame(const XMMATRIX& transform);
	std::wstring GetAnimationName();
	const XMMATRIX& GetKeyFrameTransform(int keyFrame) const;
	const ModelAnimationQuaternionKeyFrame& GetQuaternionKeyFrame(int keyFrame) const;
};

