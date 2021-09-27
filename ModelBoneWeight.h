#pragma once
#include "DXHeader.h"
#include "ModelVertexWeight.h"
#include <vector>

class ModelBoneWeight
{
private:
	float sumWeight;

	std::vector<std::pair<int, int>> boneWeights;
public:
	ModelBoneWeight();
	ModelBoneWeight(const ModelBoneWeight& other);
	~ModelBoneWeight();

	static const UINT MaxBonesPerVertex;
	static const UINT MaxBonesPerMax;

	void AddBoneWeight(int boneIndex, float boneWeight);
	void AddBoneWeight(std::pair<int, float> boneWeightPair);
	void AddBoneWeights(const ModelBoneWeight& boneWeights);

	void Validate();
	void Normalize();

	int GetBoneWeightCount() const;
	std::pair<int, float> GetBoneWeight(int index) const;
	ModelVertexWeight GetVertexWeights();
};

