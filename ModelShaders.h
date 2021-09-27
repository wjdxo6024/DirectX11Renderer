#pragma once
#ifndef _MODELSHADER_H_
#define _MODELSHADER_H_

#include <typeinfo>
#include "Camera.h"
#include "DirectXEngine.h"

#include "DXHeader.h"
#include "BumpMapShaderClass.h"
#include "TextureShaderClass.h"
#include "SpecMapShaderClass.h"
#include "ColorShaderClass.h"
#include "AlphaMapShaderClass.h"
#include "LightShaderClass.h"
#include "LightMapShaderClass.h"

class ModelShaders
{
private:
	BumpMapShaderClass* m_BumpMapShader;
	TextureShaderClass* m_TextureShader;
	SpecMapShaderClass* m_SpecMapShader;
	ColorShaderClass* m_ColorShader;
	AlphaMapShaderClass* m_AlphaMapShaderClass;

	Camera* externCamera;
	DirectXEngine* externDXEngine;
private:

public:
	bool Init(DirectXEngine*, HWND hWnd, Camera* camera);

	bool Render();
	bool RenderBump(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj,
		ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor);
	bool RenderTexture(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	bool RenderSpec(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj,
		ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);
	bool RenderColor(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj);
	bool RenderAlpha(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj, ID3D11ShaderResourceView** textureArray);

	bool Update();
	void Quit();

	void SetCamera(Camera* camera);

	BumpMapShaderClass* GetBumpShader();
};

#endif

