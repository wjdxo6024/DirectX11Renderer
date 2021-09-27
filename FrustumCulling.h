#pragma once
#ifndef _FRUSTUMCULLING_H_
#define _FRUSTUMCULLING_H_
#include "DXHeader.h"

class FrustumCulling
{
private:
	XMVECTOR m_planes[6];

public:
	FrustumCulling();
	FrustumCulling(const FrustumCulling& other);
	~FrustumCulling();

	void InitFrustum(float, XMMATRIX, XMMATRIX);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);
};

#endif
