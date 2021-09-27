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
	XMMATRIX rot; // ���ʹϿ�
	XMFLOAT4 trans; // �̵�
	XMFLOAT4 scale; // ������
};

struct SkeletonPose
{
	Skeleton* skeleton;
	JointPose* localPose;
	XMMATRIX globalPose;
};

