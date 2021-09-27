#include "FrustumCulling.h"

FrustumCulling::FrustumCulling()
{

}

FrustumCulling::FrustumCulling(const FrustumCulling& other)
{

}

FrustumCulling::~FrustumCulling()
{

}

void FrustumCulling::InitFrustum(float screenDepth, XMMATRIX projMatrix, XMMATRIX viewMatrix)
{
	// proj ����� XMFLOAT4X4�� ��ȯ
	XMFLOAT4X4 pMatrix;
	XMStoreFloat4x4(&pMatrix, projMatrix);

	// ����ü���� �ּ� Z �Ÿ��� ���
	float zMinimum = -pMatrix._43 / pMatrix._33;
	float r = screenDepth / (screenDepth - zMinimum);

	// ������Ʈ �� ���� �ٽ� proj ��Ŀ� ����
	pMatrix._33 = r;
	pMatrix._43 = -r * zMinimum;
	projMatrix = XMLoadFloat4x4(&pMatrix);

	// view��Ʈ������ ������Ʈ �� proj ��Ʈ�������� ����ü ��Ʈ������ ����
	XMMATRIX frustumMatrix = XMMatrixMultiply(viewMatrix, projMatrix);
	
	// ���� ����� XMFLOAT4X4 �������� ��ȯ
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, frustumMatrix);

	// ����ü�� ����� ����� ���
	float x = (float)(matrix._14 + matrix._13);
	float y = (float)(matrix._24 + matrix._23);
	float z = (float)(matrix._34 + matrix._33);
	float w = (float)(matrix._44 + matrix._43);

	m_planes[0] = XMVectorSet(x, y, z, w);
	m_planes[0] = XMPlaneNormalize(m_planes[0]);

	// ����ü�� �� ��� ���
	x = (float)(matrix._14 - matrix._13);
	y = (float)(matrix._24 - matrix._23);
	z = (float)(matrix._34 - matrix._33);
	w = (float)(matrix._44 - matrix._43);
	
	m_planes[1] = XMVectorSet(x, y, z, w);
	m_planes[1] = XMPlaneNormalize(m_planes[1]);

	// ����ü�� ���� ���
	x = (float)(matrix._14 + matrix._11);
	y = (float)(matrix._24 + matrix._21);
	z = (float)(matrix._34 + matrix._31);
	w = (float)(matrix._44 + matrix._41);

	m_planes[2] = XMVectorSet(x, y, z, w);
	m_planes[2] = XMPlaneNormalize(m_planes[2]);

	// ����ü�� ������ ���
	x = (float)(matrix._14 - matrix._11);
	y = (float)(matrix._24 - matrix._21);
	z = (float)(matrix._34 - matrix._31);
	w = (float)(matrix._44 - matrix._41);

	m_planes[3] = XMVectorSet(x, y, z, w);
	m_planes[3] = XMPlaneNormalize(m_planes[3]);

	// ����ü�� �� ���
	x = (float)(matrix._14 - matrix._12);
	y = (float)(matrix._24 - matrix._22);
	z = (float)(matrix._34 - matrix._32);
	w = (float)(matrix._44 - matrix._42);

	m_planes[4] = XMVectorSet(x, y, z, w);
	m_planes[4] = XMPlaneNormalize(m_planes[4]);

	// ����ü�� �Ʒ� ���
	x = (float)(matrix._14 + matrix._12);
	y = (float)(matrix._24 + matrix._22);
	z = (float)(matrix._34 + matrix._32);
	w = (float)(matrix._44 + matrix._42);

	m_planes[5] = XMVectorSet(x, y, z, w);
	m_planes[5] = XMPlaneNormalize(m_planes[5]);
}

// �� �ϳ��� �ø��Ǵ��� Ȯ��
bool FrustumCulling::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i < 6; i++)
	{
		// ����Ʈ�� �� frustum�� 6�� ��� ��� �ȿ� �ִ��� Ȯ��
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(x, y, z, 1.0f))) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

// ť�갡 �ø��Ǵ��� Ȯ��
bool FrustumCulling::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		// 1.
		if(XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius),
			1.0f))) >= 0.0f)
			continue;

		// 2.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius),
			1.0f))) >= 0.0f)
			continue;

		// 3.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius),
			1.0f))) >= 0.0f)
			continue;

		// 4.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius),
			1.0f))) >= 0.0f)
			continue;

		// 5.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius),
			1.0f))) >= 0.0f)
			continue;

		// 6.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius),
			1.0f))) >= 0.0f)
			continue;

		// 7.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius),
			1.0f))) >= 0.0f)
			continue;

		// 8.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius),
			1.0f))) >= 0.0f)
			continue;

		return false;
	}
	return true;
}

// ��ü�� �ø��Ǵ��� Ȯ��
bool FrustumCulling::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter, yCenter, zCenter, 1.0f))) < -radius)
			return false;
	}
	return true;
}

// �簢���� �ø��Ǵ��� Ȯ��
bool FrustumCulling::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	for (int i = 0; i < 6; i++)
	{
		// 1.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 1.0f)
			continue;

		// 2.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 1.0f)
			continue;

		// 3.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 1.0f)
			continue;

		// 4.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 1.0f)
			continue;

		// 5.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 1.0f)
			continue;

		// 6.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 1.0f)
			continue;

		// 7.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 1.0f)
			continue;

		// 8.
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 1.0f)
			continue;

		return false;
	}

	return true;
}