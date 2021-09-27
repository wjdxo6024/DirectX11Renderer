#include "ModelList.h"
#include <time.h>

ModelList::ModelList()
{

}

ModelList::ModelList(const ModelList& other)
{

}

ModelList::~ModelList()
{

}

bool ModelList::Init(int numModels)
{
	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;

	m_modelCount = numModels;

	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if (!m_ModelInfoList)
	{
		return false;
	}

	srand((unsigned int)time(NULL));

	for (int i = 0; i < m_modelCount; i++)
	{
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		m_ModelInfoList[i].color = XMFLOAT4(red, green, blue, 1.0f);

		// 모델에 대한 뷰어 앞에 임의의 위치를 생성
		m_ModelInfoList[i].positionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionZ = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
	}

	return true;
}

void ModelList::Quit()
{
	if (m_ModelInfoList)
	{
		delete[] m_ModelInfoList;
		m_ModelInfoList = 0;
	}
}

int ModelList::GetModelCount()
{
	return m_modelCount;
}

void ModelList::GetData(int index, float& positionX, float& positionY, float& positionZ, XMFLOAT4& color)
{
	if (index >= m_modelCount)
		return;

	positionX = m_ModelInfoList[index].positionX;
	positionY = m_ModelInfoList[index].positionY;
	positionZ = m_ModelInfoList[index].positionZ;

	color = m_ModelInfoList[index].color;
}