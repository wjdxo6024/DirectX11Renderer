#pragma once
#ifndef _SKELETONANIMATION_H_
#define _SKELETONANIMATION_H_
#include "DXHeader.h"
#define MAX_FRAME_NAME 100
struct AnimationFrame
{
	char Name[MAX_FRAME_NAME];
	UINT Mesh;
	UINT ParentFrame;
	UINT ChildFrame;
	UINT SiblingFrame;
	XMMATRIX Matrix;
	UINT AnimationDataIndex;
};

struct AnimationData
{
	XMFLOAT3 Translation;
	XMFLOAT4 Orientation;
	XMFLOAT3 Scaling;
};

class SkeletonAnimation
{
private:
	
public:
	SkeletonAnimation();
	SkeletonAnimation(const SkeletonAnimation& other);
	~SkeletonAnimation();

	bool Init();
	bool Quit();

	bool CreateFrame();
	bool CreateHierarchy();
};

#endif

