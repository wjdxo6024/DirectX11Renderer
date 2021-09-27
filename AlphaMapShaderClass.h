#pragma once
#ifndef _ALPHAMAPSHADER_H_
#define _ALPHAMAPSHADER_H_
#include "DXHeader.h"

class AlphaMapShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void Shutdown();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**);
	void RenderShader(ID3D11DeviceContext*, int);

public:
	AlphaMapShaderClass();
	AlphaMapShaderClass(const AlphaMapShaderClass& other);
	~AlphaMapShaderClass();

	bool Init(ID3D11Device*, HWND);
	void Quit();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**);
};


#endif