#pragma once
#ifndef _CAMERAPOSITION_H_
#define _CAMERAPOSITION_H_
#include "DXHeader.h"

class CameraPosition
{
private:
	float m_frameTime = 0.0f;
	float m_rotationY = 0.0f;
	float m_leftTurnSpeed = 0.0f;
	float m_rightTurnSpeed = 0.0f;
public:
	CameraPosition();
	CameraPosition(const CameraPosition& other);
	~CameraPosition();

	void SetFrameTime(float);
	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);
};

#endif