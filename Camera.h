#pragma once
#include "DXHeader.h"
class Camera : public AlignedAllocationPolicy<16>
{
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projMatrix;

	XMFLOAT3 m_Cross;
	XMFLOAT3 m_View;

	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	DWORD m_dwMouseX;
	DWORD m_dwMouseY;

	RECT rc;
	HWND m_hWnd;

public:
	Camera();
	Camera(const Camera&);
	~Camera();

	bool Init(ID3D11Device*, HWND);
	void Update(ID3D11Device*);
	void Quit();

	void Render();
	void GetViewMatrix(XMMATRIX&);

	//XMMATRIX GetProj();
	//XMFLOAT3 GetEye();
	//XMFLOAT3 GetAt();
	//XMFLOAT3 GetUp();

	//XMFLOAT3 GetCross();
	//XMFLOAT3 GetvView();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	//void MoveLocalZ(float dist);
	//void MoveLocalX(float dist);

	//void RotationLocalY(float angle);
	//void RotationLocalX(float angle);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
};

