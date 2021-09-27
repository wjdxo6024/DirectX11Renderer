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
	// Update(int, int)�� ���콺 ������ ��ǥ�� �޴´�.
	void Render(); // Begin - Render - End �������� ������
	void Begin();
	void End();

	void Update();
	void Quit();

	DirectXEngine* GetDirectXEngine();
	Direct2DEngine* GetDirect2DEngine();
};

