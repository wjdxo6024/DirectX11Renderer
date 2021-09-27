#include "TextureArray.h"

TextureArray::TextureArray()
{

}

TextureArray::TextureArray(const TextureArray & other)
{

}

TextureArray::~TextureArray()
{

}

bool TextureArray::Init(ID3D11Device* device, WCHAR* textureFile)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_Textures[i] != nullptr)
			continue;

		if (FAILED(CreateDDSTextureFromFile(device, textureFile, nullptr, &m_Textures[i])))
		{
#ifdef _DEBUG
			MessageBox(NULL, L"Error", L"TextureArray 1 : Cannot Creat Texture.", MB_OK);
#endif
			return false;
		}
		else
		{
			break;
		}
	}
	return true;
}

bool TextureArray::Init(ID3D11Device* device, WCHAR* textureFile1, WCHAR* textureFile2)
{
	if (FAILED(CreateDDSTextureFromFile(device, textureFile1, nullptr, &m_Textures[0])))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Error", L"TextureArray 1 : Cannot Creat Texture.", MB_OK);
#endif
		return false;
	}

	if (FAILED(CreateDDSTextureFromFile(device, textureFile2, nullptr, &m_Textures[1])))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Error", L"TextureArray 2 : Cannot Creat Texture.", MB_OK);
#endif
		return false;
	}

	return true;
}

bool TextureArray::Init(ID3D11Device* device, int args, ...)
{
	va_list fileName;

	va_start(fileName, args);
	for (int i = 0; i < args; i++)
	{
		WCHAR* textureFile = va_arg(fileName, WCHAR*);
		if (FAILED(CreateDDSTextureFromFile(device, textureFile, nullptr, &m_Textures[i])))
		{
#ifdef _DEBUG
			MessageBox(NULL, L"Error", L"TextureArray : Cannot Creat Texture.", MB_OK);
#endif
			return false;
		}
	}

	va_end(fileName);

	return true;
}

void TextureArray::Quit()
{
	if (m_Textures[0])
	{
		m_Textures[0]->Release();
		m_Textures[0] = 0;
	}

	if (m_Textures[1])
	{
		m_Textures[1]->Release();
		m_Textures[1] = 0;
	}

	if (m_Textures[2])
	{
		m_Textures[2]->Release();
		m_Textures[2] = 0;
	}

	return;
}

ID3D11ShaderResourceView** TextureArray::GetTextureArray()
{
	return m_Textures;
}