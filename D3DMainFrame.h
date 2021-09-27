#pragma once
#include "DirectXEngine.h"
#include "Direct2DEngine.h"

class D3DMainFrame
{
private:
	DirectXEngine* m_DXEngine = nullptr;
	Direct2DEngine* m_2DEngine = nullptr;

public:
	void Init(int width, int height, HWND hWnd, bool fullscreen);
	void Run();
	// Update(int, int)는 마우스 포인터 좌표를 받는다.
	void Render(); // Begin - Render - End 형식으로 렌더링
	void Begin();
	void End();

	void Update();
	void Quit();

	DirectXEngine* GetDirectXEngine();
	Direct2DEngine* GetDirect2DEngine();
};

