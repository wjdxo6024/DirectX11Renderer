#include "TestMulitTextureScene.h"

TestMulitTextureScene::TestMulitTextureScene()
{

}

TestMulitTextureScene::TestMulitTextureScene(const TestMulitTextureScene& other)
{

}

TestMulitTextureScene::~TestMulitTextureScene()
{

}

bool TestMulitTextureScene::Init(DirectXEngine* DXEngine, Direct2DEngine* D2DEngine, HWND hWnd, int width, int height)
{
	m_ParentTimer = NULL;

	m_Camera = new Camera();
	if (m_Camera->Init(DXEngine->GetDevice(), hWnd) == false)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Camera From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -3.0f);

	m_CameraPosition = new CameraPosition();

	char modelName[] = "./Model/DragonTest1.fbx";

	m_ModelShader = new ModelShaders();
	if (!m_ModelShader->Init(DXEngine, hWnd, m_Camera))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot create ModelShader.", L"Error", MB_OK);
#endif
		return false;
	}


	m_ModelScene = new ModelScene;
	if (!m_ModelScene->Init(DXEngine, m_Camera, m_ModelShader, modelName))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot create ModelScene.", L"Error", MB_OK);
#endif
		return false;
	}

	m_Light = new Light();

	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_Light->SetDirection(1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularColor(0.5f, 0.5f, 0.5f, 0.5f);
	m_Light->SetSpecularPower(1000.0f);
	m_ModelScene->PushLight(m_Light);

	// RTT
	m_RenderTexture = new RenderTexture();
	if (!m_RenderTexture->Init(DXEngine->GetDevice(), width, height))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot create Render Texture.", L"Error", MB_OK);
#endif
		return false;
	}

	m_RenderTextureDrawer = new RenderTargetDrawer();
	if (!m_RenderTextureDrawer->Init(DXEngine->GetDevice(), width, height, 100, 100))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot create Render Texture Drawer.", L"Error", MB_OK);
#endif
		return false;
	}
	return true;
}

bool TestMulitTextureScene::Init(DirectXEngine* DXEngine, Direct2DEngine* D2DEngine, HWND hWnd, int width, int height, Timer* parentTimer)
{
	this->Init(DXEngine, D2DEngine, hWnd, width, height);
	m_ParentTimer = parentTimer;

	return true;
}

// TODO : 구조 개선 요구, 렌더 텍스처는 BeginScene으로 초기화를 하면서 해야하지만 그러지 못함.

bool TestMulitTextureScene::PreRender(DirectXEngine* DXEngine)
{
	m_RenderTexture->SetRenderTarget(DXEngine->GetDeviceContext(), DXEngine->GetDepthStencilView());
	m_RenderTexture->ClearRenderTarget(DXEngine->GetDeviceContext(), DXEngine->GetDepthStencilView(),
		0.0f, 1.0f, 1.0f, 1.0f);
	//-----------------------------------------------
	if (rotation > 360.0f)
		rotation = 0.0f;
	// 카메라의 위치에 따라 뷰 행렬을 생성
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투형 행렬을 가져옴
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	DXEngine->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	DXEngine->GetProjectionMatrix(projectionMatrix);
	DXEngine->GetOrthoMatrix(orthoMatrix);

	/*m_ModelBump->Render(DXEngine->GetDeviceContext());*/


	// ----------------------------------------------

	DXEngine->SetBackBufferRenderTarget();

	return true;
}

bool TestMulitTextureScene::Render(DirectXEngine* DXEngine)
{
	if (rotation > 360.0f)
		rotation = 0.0f;
	// 카메라의 위치에 따라 뷰 행렬을 생성
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투형 행렬을 가져옴
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	DXEngine->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	DXEngine->GetProjectionMatrix(projectionMatrix);
	DXEngine->GetOrthoMatrix(orthoMatrix);

	worldMatrix = worldMatrix * XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixRotationY(XMConvertToRadians(rotation));
	m_ModelScene->Render(worldMatrix, viewMatrix, projectionMatrix);
	rotation += 0.5f;

	DXEngine->TurnZBufferOff();

	// RTT

	m_Camera->GetViewMatrix(viewMatrix);
	DXEngine->GetWorldMatrix(worldMatrix);
	DXEngine->GetOrthoMatrix(orthoMatrix);

	if (!m_RenderTextureDrawer->Render(DXEngine->GetDeviceContext(), 100, 100))
	{
		return false;
	}


	DXEngine->TurnZBufferOn();

	return true;
}

bool TestMulitTextureScene::AfterRender(DirectXEngine* DXEngine)
{
	return true;
}

bool TestMulitTextureScene::Update(DirectXEngine* DXEngine)
{
	return true;
}

void TestMulitTextureScene::Quit()
{
	// RTT
	if (m_RenderTextureDrawer)
	{
		m_RenderTextureDrawer->Quit();
		delete m_RenderTextureDrawer;
		m_RenderTextureDrawer = 0;
	}

	if (m_RenderTexture)
	{
		m_RenderTexture->Quit();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Shader
	if (m_ModelShader)
	{
		m_ModelShader->Quit();
		delete m_ModelShader;
		m_ModelShader = 0;
	}
	// Models
	if (m_ModelScene)
	{
		m_ModelScene->Quit();
		delete m_ModelScene;
		m_ModelScene = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

}

bool TestMulitTextureScene::MouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool TestMulitTextureScene::KeyBoardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z + 1.0f);
		break;
	case VK_DOWN:
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z - 1.0f);
		break;

	}

	return true;
}