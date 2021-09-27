#include "D3DMainFrame.h"

void D3DMainFrame::Init(int width, int height, HWND hWnd, bool fullscreen)
{
	m_DXEngine = new DirectXEngine(width, height);
	if (m_DXEngine->Init(width, height, hWnd, fullscreen) != true)
	{
		MessageBox(NULL, L"DirectX Initialize Failed.", L"Error", MB_OK);
		return;
	}

	m_2DEngine = new Direct2DEngine();
	if (m_2DEngine->Init(m_DXEngine, hWnd, width, height, fullscreen) != true)
	{
		MessageBox(NULL, L"Direct2D Initialize Failed.", L"Error", MB_OK);
		return;
	}

	//m_Scene = new TestScene();
	//if (m_Scene->Init(m_DXEngine, m_2DEngine, hWnd, width, height) != true)
	//{
	//	MessageBox(NULL, L"Scene Loading Failed.", L"Error", MB_OK);
	//	return;
	//}
}

void D3DMainFrame::Run()
{
	m_DXEngine->BeginScene();

	//m_2DEngine->Render();

	m_2DEngine->BeginDraw();


	m_2DEngine->EndDraw();

	// 버퍼의 내용을 화면에 출력
	m_DXEngine->EndScene();

	return;
}

void D3DMainFrame::Begin()
{
	m_DXEngine->BeginScene();

	m_2DEngine->BeginDraw();
}

void D3DMainFrame::End()
{
	m_2DEngine->EndDraw();

	// 버퍼의 내용을 화면에 출력
	m_DXEngine->EndScene();
}

void D3DMainFrame::Render()
{

}

void D3DMainFrame::Update()
{
}

void D3DMainFrame::Quit()
{
	if (m_2DEngine)
	{
		m_2DEngine->Quit();
		delete m_2DEngine;
		m_2DEngine = 0;
	}

	if (m_DXEngine)
	{
		m_DXEngine->Quit();
		delete m_DXEngine;
		m_DXEngine = 0;
	}
}

DirectXEngine* D3DMainFrame::GetDirectXEngine()
{
	return m_DXEngine;
}

Direct2DEngine* D3DMainFrame::GetDirect2DEngine()
{
	return m_2DEngine;
}