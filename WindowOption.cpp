#include "WindowOption.h"

WindowOption::WindowOption()
{
	m_WindowWidth = GetSystemMetrics(SM_CXSCREEN);
	m_WindowHeight = GetSystemMetrics(SM_CYSCREEN);
	FullScreen = false;
}

void WindowOption::setHeight(int height)
{
	m_WindowHeight = height;
}

void WindowOption::setWidth(int width)
{
	m_WindowWidth = width;
}

int WindowOption::getHeight()
{
	return m_WindowHeight;
}

int WindowOption::getWidth()
{
	return m_WindowWidth;
}

void WindowOption::setFullScreen(bool fullScreen)
{
	FullScreen = fullScreen;
}

bool WindowOption::getFullScreen()
{
	return FullScreen;
}