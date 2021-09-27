#include "ModelBoneWeight.h"

ModelBoneWeight::ModelBoneWeight()
{

}

ModelBoneWeight::ModelBoneWeight(const ModelBoneWeight& other)
{

}

ModelBoneWeight::~ModelBoneWeight()
{

}

void ModelBoneWeight::AddBoneWeight(int boneIndex, float boneWeight)
{

}

void ModelBoneWeight::AddBoneWeight(std::pair<int, float> boneWeightPair)
{

}

void ModelBoneWeight::AddBoneWeights(const ModelBoneWeight& boneWeights)
{

}

void ModelBoneWeight::Validate()
{

}

void ModelBoneWeight::Normalize()
{

}

int ModelBoneWeight::GetBoneWeightCount() const
{
	return 0;
}

std::pair<int, float> ModelBoneWeight::GetBoneWeight(int index) const
{
	return std::make_pair(0, 0.0f);
}

ModelVertexWeight ModelBoneWeight::GetVertexWeights()
{
	return ModelVertexWeight();
}