#include "Light.h"

Light::Light()
{

}

Light::Light(const Light& other)
{

}

Light::~Light()
{

}

void Light::SetLightPos(float x, float y, float z)
{
	m_LightPos.x = x;
	m_LightPos.y = y;
	m_LightPos.z = z;
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetSpecularPower(float power)
{
	m_specularPower = power;
}

XMFLOAT4 Light::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 Light::GetDirection()
{
	return m_direction;
}

XMFLOAT4 Light::GetSpecularColor()
{
	return m_specularColor;
}

XMFLOAT3 Light::GetLightPos()
{
	return m_LightPos;
}

float Light::GetSpecularPower()
{
	return m_specularPower;
}