#pragma once
#include "DXHeader.h"
class ModelVertexWeight
{
private:
	XMFLOAT4 VertexIndices;
	XMFLOAT4 VertexWeight;

public:
	ModelVertexWeight()
	{
		VertexIndices = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		VertexWeight = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void SetVertexWeight(int Index, int BoneIndex, float Weight)
	{
		switch (Index)
		{
		case 0:
			VertexIndices.x = (float)BoneIndex;
			VertexWeight.x = Weight;
			break;
		case 1:
			VertexIndices.y = (float)BoneIndex;
			VertexWeight.y = Weight;
			break;
		case 2:
			VertexIndices.z = (float)BoneIndex;
			VertexWeight.z = Weight;
			break;
		case 3:
			VertexIndices.w = (float)BoneIndex;
			VertexWeight.w = Weight;
			break;
		}
	}
};

