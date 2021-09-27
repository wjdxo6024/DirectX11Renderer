#include "GameScene.h"

// �⺻���� ���� �ʱ�ȭ
bool GameScene::Init()
{
	return true;
}

bool GameScene::Init(DirectXEngine* dxEngine, Direct2DEngine* dx2dEngine, HWND hWnd, int width, int height)
{
	IsChangeScene = false;

	m_dxEngine = dxEngine;
	m_2dEngine = dx2dEngine;

	// System �ʱ�ȭ
	m_SystemManager.Init();
	m_SystemText = new Text();

	// Fps �� cpu �ؽ�Ʈ �ʱ�ȭ
	wcscpy(m_FpsText, L"");
	wcscpy(m_CpuText, L"");

	if (m_SystemText->Init(dx2dEngine, m_FpsText) != true)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Text Create Failed.", L"Error", MB_OK);
#endif
		return false;
	}
	m_SystemText->SetFontColor(D2D1::ColorF(0.1f, 1.0f, 0.1f, 1.0f));
	
	// Direct2D Test Init
	m_Ellipse = new CEllipse();
	m_Ellipse->Init(m_2dEngine, 100, 300, 50, 100);
	m_Ellipse->SetRotationDegree(20.0f);
	
	m_Bitmap = new CBitmap();
	WCHAR bitmapPath[] = L"./Image/ButtonPreviousicon.png";
	m_Bitmap->Init(m_2dEngine, 100, 500, 100, 100, bitmapPath);
	
	// ���⼭ ó�� ���� �ʱ�ȭ�� ����Ѵ�.
	// SceneState�� ���� �ʱ�ȭ�Ǵ� ���� �ٲ�� �Ѵ�.

	m_TestScene = NULL; // m_TestScene ������� ����
//	m_TestScene = new TestScene();
//	if (m_TestScene->Init(dxEngine, dx2dEngine, hWnd, width, height) != true)
//	{
//#ifdef _DEBUG
//		MessageBox(NULL, L"Scene Loading Failed.", L"Error", MB_OK);
//#endif
//		return false;
//	}

	m_TestMultiTextureScene = NULL;
	m_TestMultiTextureScene = new TestMulitTextureScene();
	if (m_TestMultiTextureScene->Init(dxEngine, dx2dEngine, hWnd, width, height) != true)
	{
	#ifdef _DEBUG
		MessageBox(NULL, L"TestMultiTextureScene Loading Failed.", L"Error", MB_OK);
#endif
		return false;
	}
	return true;
}

bool GameScene::PreRender()
{
	if (m_TestMultiTextureScene != NULL)
	{
		m_TestMultiTextureScene->PreRender(m_dxEngine);
	}
	
	return true;
}

// �������� �𵨸��� �����ϴ� ���̴�.
bool GameScene::Render()
{
	if (m_TestScene != NULL)
	{
		m_TestScene->Render(m_dxEngine);
	}

	if (m_TestMultiTextureScene != NULL)
	{
		m_TestMultiTextureScene->Render(m_dxEngine);
	}

	// FPS, CPU ���
	m_SystemText->DrawDirect(2, 5, m_FpsText);
	m_SystemText->DrawDirect(2, 30, m_CpuText);

	//m_Ellipse->Draw();
	//m_Bitmap->Draw();
	return true;
}

bool GameScene::AfterRender()
{
	if (m_TestMultiTextureScene != NULL)
	{
		m_TestMultiTextureScene->AfterRender(m_dxEngine);
	}
	return true;
}

// ������Ʈ ���� �޽����� ó���Ѵ�.
bool GameScene::Update()
{
	if (m_TestScene != NULL)
	{
		m_TestScene->Update(m_dxEngine);
	}

	m_SystemManager.GetFpsCounter()->Frame();
	m_SystemManager.GetCpuCounter()->Frame();
	m_SystemManager.GetTimer()->Frame();

	swprintf(m_FpsText, L"FPS: %d", m_SystemManager.GetFpsCounter()->GetFps());
	swprintf(m_CpuText, L"CPU: %d", m_SystemManager.GetCpuCounter()->GetCpuPercentage());

	return true;
}

// ������ ����ǰų�
bool GameScene::Release()
{
	if (m_TestMultiTextureScene != NULL)
	{
		m_TestMultiTextureScene->Quit();
		delete m_TestMultiTextureScene;
	}
	if (m_TestScene != NULL)
	{
		m_TestScene->Quit();
		delete m_TestScene;
	}

	if (m_SystemText != NULL)
	{
		m_SystemText->Quit();
		delete m_SystemText;
	}

	if (m_Ellipse != NULL)
	{
		m_Ellipse->Quit();
		delete m_Ellipse;
	}

	if (m_Bitmap != NULL)
	{
		m_Bitmap->Quit();
		delete m_Bitmap;
	}
	return true;
}

bool GameScene::MouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_TestScene != NULL)
	{
		m_TestScene->MouseEvent(message, wParam, lParam);
	}
	return true;
}

bool GameScene::KeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_TestScene != NULL)
	{
		m_TestScene->KeyBoardEvent(message, wParam, lParam);
	}

	if (m_TestMultiTextureScene != NULL)
	{
		m_TestMultiTextureScene->KeyBoardEvent(message, wParam, lParam);
	}
	return true;
}