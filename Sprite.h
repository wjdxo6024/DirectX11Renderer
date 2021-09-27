#pragma once
#include "DXHeader.h"
#include "Texture.h"
#include <wrl/client.h>

// TODO : 아시다시피 스프라이트는 버튼, 게이지등 여러 애니메이션을 포함해야하기 때문에 이에 맞춘
//		  클래스변경이 필요함.
// BITMAP으로 대체 됨.
class Sprite
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	Texture* m_Texture = nullptr;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_spriteWidth = 0;
	int m_spriteHeight = 0;
	int m_prePosX = 0;
	int m_prePosY = 0;
public:
	Sprite();
	Sprite(const Sprite&);
	~Sprite();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, int, int, WCHAR*, int, int);
	void Quit();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitBuffers(ID3D11Device*);
	void Shutdown();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void ReleaseTexture();
};

