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
	// 위쪽을 가리키는 벡터를 설정
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// XMVECTOR 구조체에 로드
	upVector = XMLoadFloat3(&up);

	// 3D월드에서 카메라의 위치 설정
	position = m_position;

	// XMVECTOR 구조체에 로드
	positionVector = XMLoadFloat3(&position);

	// 기본적으로 카메라가 찾고있는 위치를 설정
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// XMVECTOR 구조체에 로드
	lookAtVector = XMLoadFloat3(&lookAt);

	// yaw, pitch, roll 의 회전값을 라디안 단위로 설정
	pitch = XMConvertToRadians(m_rotation.x);
	yaw = XMConvertToRadians(m_rotation.y);
	roll = XMConvertToRadians(m_rotation.z);

	// yaw, pitch, roll 값을 통해 회전 행렬을 만듬
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt 및 up 벡터를 회전 행렬로 변형하여 뷰가 원점에서 올바르게 회전
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// 회전 된 카메라 위치를 뷰어 위치로 변환
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// 마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬 제작
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(XMMATRIX& view)
{
	view = m_viewMatrix;
}

void Camera::Update(ID3D11Device* device)
{

}