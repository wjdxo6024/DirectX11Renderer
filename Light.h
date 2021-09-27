#pragma once
#include "DXHeader.h"

class Light
{
public: // Light Option
	
private:
	XMFLOAT3 m_LightPos;

	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_specularColor;
	float m_specularPower;

public:
	Light();
	Light(const Light&);
	~Light();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	void SetLightPos(float, float, float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetSpecularColor();
	XMFLOAT3 GetLightPos();
	float GetSpecularPower();

};

