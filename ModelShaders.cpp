#include "ModelShaders.h"

bool ModelShaders::Init(DirectXEngine* DXEngine, HWND hWnd, Camera* camera)
{
	externCamera = camera;
	externDXEngine = DXEngine;

	m_SpecMapShader = new SpecMapShaderClass();

	if (!m_SpecMapShader->Init(externDXEngine->GetDevice(), hWnd))
	{
		return false;
	}

	m_BumpMapShader = new BumpMapShaderClass();
	if (!m_BumpMapShader->Init(externDXEngine->GetDevice(), hWnd))
	{
		return false;
	}

	m_ColorShader = new ColorShaderClass();
	if (!m_ColorShader->Init(externDXEngine->GetDevice(), hWnd))
	{
		return false;
	}

	m_AlphaMapShaderClass = new AlphaMapShaderClass();
	if (!m_AlphaMapShaderClass->Init(externDXEngine->GetDevice(), hWnd))
	{
		return false;
	}

	m_TextureShader = new TextureShaderClass();
	if (!m_TextureShader->Init(externDXEngine->GetDevice(), hWnd))
	{
		return false;
	}
	return true;
}

bool ModelShaders::Render()
{
	
	return true;
}

bool ModelShaders::RenderBump(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	m_BumpMapShader->Render(deviceContext, indexCount, world, view, proj, textureArray, lightDirection, diffuseColor);
	return true;
}

bool ModelShaders::RenderTexture(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	m_TextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix
	, projectionMatrix, texture);
	return true;
}

bool ModelShaders::RenderSpec(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower)
{
	m_SpecMapShader->Render(deviceContext, indexCount, world, view, proj, textureArray, lightDirection, diffuseColor, cameraPosition, specularColor, specularPower);
	return true;
}

bool ModelShaders::RenderColor(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	m_ColorShader->Render(deviceContext, indexCount, world, view, proj);
	return true;
}

bool ModelShaders::RenderAlpha(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj, ID3D11ShaderResourceView** textureArray)
{
	m_AlphaMapShaderClass->Render(deviceContext, indexCount, world, view, proj, textureArray);
	return true;
}

bool ModelShaders::Update()
{
	return true;
}

void ModelShaders::Quit()
{
	if (m_TextureShader != NULL)
	{
		m_TextureShader->Quit();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_AlphaMapShaderClass != NULL)
	{
		m_AlphaMapShaderClass->Quit();
		delete m_AlphaMapShaderClass;
		m_AlphaMapShaderClass = 0;
	}

	if (m_ColorShader != NULL)
	{
		m_ColorShader->Quit();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_BumpMapShader != NULL)
	{
		m_BumpMapShader->Quit();
		delete m_BumpMapShader;
		m_BumpMapShader = 0;
	}

	if (m_SpecMapShader != NULL)
	{
		m_SpecMapShader->Quit();
		delete m_SpecMapShader;
		m_SpecMapShader = 0;
	}
}

void ModelShaders::SetCamera(Camera* camera)
{
	externCamera = camera;
}

BumpMapShaderClass* ModelShaders::GetBumpShader()
{
	return m_BumpMapShader;
}