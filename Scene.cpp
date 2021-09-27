#include "Scene.h"

LRESULT CALLBACK Scene::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_LBUTTONDOWN:
	{
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_END)
		{
			if (MessageBox(NULL, TEXT("Quit?"), TEXT("End"), MB_YESNO) == true)
			{
				PostQuitMessage(0);
				return 0;
			}
		}
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}