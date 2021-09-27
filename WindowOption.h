#pragma once
#include <Windows.h>

class WindowOption
{
private:
	int m_WindowHeight;
	int m_WindowWidth;
	bool FullScreen;
public:
	WindowOption();

	void setHeight(int height);
	void setWidth(int width);
	int getHeight();
	int getWidth();

	void setFullScreen(bool fullScreen);
	bool getFullScreen();
};