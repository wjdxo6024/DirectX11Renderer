#pragma once
#include "DXHeader.h"
#include <dinput.h>
class InputController
{
private:
	IDirectInput8* m_directInput = nullptr;
	IDirectInputDevice8* m_keyboard = nullptr;
	IDirectInputDevice8* m_mouse = nullptr;

	unsigned char m_keyboardState[256] = { 0, };
	DIMOUSESTATE m_mouseState;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

public:
	InputController();
	InputController(const InputController&);
	~InputController();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Quit();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	char GetKeyBoardBuffer();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
};