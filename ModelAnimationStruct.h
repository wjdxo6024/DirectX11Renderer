#pragma once
#include <vector>
#include <string>
#include "DXHeader.h"

struct Joint
{
	std::string jointName;
	int parentNode;
	XMMATRIX invBindPose;
};

struct Skeleton
{
	int jointCount;
	Joint* jointArray;
};

struct JointPose
{
	XMMATRIX rot; // 쿼터니온
	XMFLOAT4 trans; // 이동
	XMFLOAT4 scale; // 스케일
};

struct SkeletonPose
{
	Skeleton* skeleton;
	JointPose* localPose;
	XMMATRIX globalPose;
};

