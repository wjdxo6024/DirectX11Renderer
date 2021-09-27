#pragma once
#include <Windows.h>
#include "DXHeader.h"
class Scene
{
public:
	Scene* nextScene;
	Scene* beforeScene;

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

