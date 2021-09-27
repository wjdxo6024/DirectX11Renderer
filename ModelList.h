#pragma once
#ifndef _MODELINFOTYPE_H_
#define _MODELINFOTYPE_H_
#include "DXHeader.h"

class ModelList
{
private:
	struct ModelInfoType
	{
		XMFLOAT4 color;
		float positionX, positionY, positionZ;
	};

private:
	int m_modelCount = 0;
	ModelInfoType* m_ModelInfoList = nullptr;

public:
	ModelList();
	ModelList(const ModelList& other);
	~ModelList();

	bool Init(int numModels);
	void Quit();

	int GetModelCount();
	void GetData(int index, float& positionX, float& positionY, float& positionZ, XMFLOAT4& color);
};

#endif

