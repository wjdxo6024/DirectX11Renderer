#pragma once
#include "DXHeader.h"

class RenderTargetDrawer
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

private:
	ID3D11Buffer *m_vertexBuffer = nullptr;
	ID3D11Buffer *m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_bitmapWidth = 0;
	int m_bitmapHeight = 0;
	int m_previousPosX = 0;
	int m_previousPosY = 0;

private:
	bool InitBuffers(ID3D11Device*);
	void Shutdown();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

public:
	RenderTargetDrawer();
	RenderTargetDrawer(const RenderTargetDrawer&);
	~RenderTargetDrawer();

	bool Init(ID3D11Device*, int, int, int, int);
	void Quit();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
};

