#pragma once
#include "DXHeader.h"

class RenderTexture
{
private:
	ID3D11Texture2D* m_renderTargetTexture = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11ShaderResourceView* m_shaderResourceView = nullptr;

public:
	RenderTexture();
	RenderTexture(const RenderTexture&);
	~RenderTexture();

	bool Init(ID3D11Device*, int, int);
	void Quit();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();
};

