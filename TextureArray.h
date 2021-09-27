#pragma once
#include "DXHeader.h"

class TextureArray
{
private:
	ID3D11ShaderResourceView* m_Textures[3] = { nullptr, nullptr, nullptr};
public:
	TextureArray();
	TextureArray(const TextureArray &);
	~TextureArray();

	bool Init(ID3D11Device*, WCHAR*);
	bool Init(ID3D11Device*, WCHAR*, WCHAR*);
	bool Init(ID3D11Device* device, int args, ...);
	void Quit();

	ID3D11ShaderResourceView** GetTextureArray();
};

