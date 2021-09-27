#pragma once

#include "WindowOption.h"
#include "Scene.h"
#include "DirectXEngine.h"
#include "Direct2DEngine.h"

#include "TestObject.h"
#include "TestTextureObject.h"
#include "TestLightObject.h"
#include "ModelObject.h"

#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "PhongShaderClass.h"

#include "Sprite.h"
#include "Text.h"

#include "Camera.h"
#include "Light.h"

#include <wchar.h>

class TestScene :
	public Scene
{
private:
	Camera* m_Camera = nullptr;

	//TODO : Scene을 만들어서 그안에 여러 오브젝트를 넣어서 실행하도록 할 것.
	TestObject* m_Triangle = nullptr;
	TestTextureObject* m_TextureTriangle = nullptr;
	TestLightObject* m_LightTriangle = nullptr;

	ModelObject* m_Model = nullptr;

	ColorShaderClass* m_ColorShader = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	LightShaderClass* m_LightShader = nullptr;
	PhongShaderClass* m_PhongShader = nullptr;

	Light* m_Light = nullptr;

	Sprite* m_TestSprite = nullptr;

	Text* m_TestText = nullptr;

	// MousePoint
	int x, y;
	WCHAR m_xText[64];
	WCHAR m_yText[64];
	float rotation = 0.0f;

	//KeyBoard
	WCHAR m_KeyboardText[64];
	std::wstring wide_string;
	std::string str;
	std::vector<WCHAR> writable;
public:
	bool Init(DirectXEngine*, Direct2DEngine*, HWND, int, int);
	bool Render(DirectXEngine*);
	bool Update(DirectXEngine*);
	void Quit();

	bool MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool KeyBoardEvent(UINT message, WPARAM wParam, LPARAM lParam);

	void GetXY(int x, int y);
	void GetInput(char);
};

