#include "Camera.h"

Camera::Camera()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Camera::Camera(const Camera& other)
{

}

Camera::~Camera()
{

}

bool Camera::Init(ID3D11Device* device, HWND hWnd)
{
	return true;
}

void Camera::Quit()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return m_position;
}

XMFLOAT3 Camera::GetRotation()
{
	return m_rotation;
}

void Camera::Render()
{
	// ������ ����Ű�� ���͸� ����
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// XMVECTOR ����ü�� �ε�
	upVector = XMLoadFloat3(&up);

	// 3D���忡�� ī�޶��� ��ġ ����
	position = m_position;

	// XMVECTOR ����ü�� �ε�
	positionVector = XMLoadFloat3(&position);

	// �⺻������ ī�޶� ã���ִ� ��ġ�� ����
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// XMVECTOR ����ü�� �ε�
	lookAtVector = XMLoadFloat3(&lookAt);

	// yaw, pitch, roll �� ȸ������ ���� ������ ����
	pitch = XMConvertToRadians(m_rotation.x);
	yaw = XMConvertToRadians(m_rotation.y);
	roll = XMConvertToRadians(m_rotation.z);

	// yaw, pitch, roll ���� ���� ȸ�� ����� ����
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt �� up ���͸� ȸ�� ��ķ� �����Ͽ� �䰡 �������� �ùٸ��� ȸ��
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// ȸ�� �� ī�޶� ��ġ�� ��� ��ġ�� ��ȯ
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// ���������� �� ���� ������Ʈ �� ���Ϳ��� �� ��� ����
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(XMMATRIX& view)
{
	view = m_viewMatrix;
}

void Camera::Update(ID3D11Device* device)
{

}