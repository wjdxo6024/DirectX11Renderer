#pragma once
#include "DXHeader.h"
#include <DDSTextureLoader.h>
#include <iostream>
class Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
private:
	unsigned char* m_targaData = nullptr;
	ID3D11Texture2D* m_texture = nullptr;
	ID3D11ShaderResourceView* m_textureView = nullptr;

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void Quit();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(char*, int&, int&);
	bool LoadPng(char*, int&, int&);
	bool LoadJpg(char*, int&, int&);
	bool LoadDDS(WCHAR*, int&, int&);
};

