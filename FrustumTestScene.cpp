#include "FrustumTestScene.h"

FrustumTestScene::FrustumTestScene()
{

}

FrustumTestScene::FrustumTestScene(const FrustumTestScene& other)
{

}

FrustumTestScene::~FrustumTestScene()
{

}

bool FrustumTestScene::Init(DirectXEngine* DXEngine, Direct2DEngine* D2DEngine, HWND hWnd, int width, int height)
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

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_CameraPosition = new CameraPosition();

	m_Model = new ModelObject();
	WCHAR ddsName[] = L"./Texture/t1.jpg";
	char modelName[] = "./Model/Sherman_fbx.fbx";
	if (!m_Model->Init(DXEngine->GetDevice(), DXEngine->GetDeviceContext(), ddsName, modelName))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Model From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_LightShader = new LightShaderClass();
	if (!m_LightShader->Init(DXEngine->GetDevice(), hWnd))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init LightShader From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_ModelList = new ModelList();
	if (!m_ModelList->Init(25))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create ModelList From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_Frustum = new FrustumCulling();

	m_Light = new Light();

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 0.15f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_FrustumSceneText = new Text();
	if (!m_FrustumSceneText->Init(D2DEngine, (WCHAR*)L""))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init Text From FrustumScene", L"Error", MB_OK);
#endif
		return false;
	}
	
	return true;
}

bool FrustumTestScene::Init(DirectXEngine* DXEngine, Direct2DEngine* D2DEngine, HWND hWnd, int width, int height, Timer* parentTimer)
{
	m_ParentTimer = parentTimer;

	m_Camera = new Camera();
	if (m_Camera->Init(DXEngine->GetDevice(), hWnd) == false)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Camera From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_CameraPosition = new CameraPosition();

	m_Model = new ModelObject();
	WCHAR ddsName[] = L"./Texture/Red.dds";
	char modelName[] = "./Model/Sherman_fbx.fbx";
	if (!m_Model->Init(DXEngine->GetDevice(), DXEngine->GetDeviceContext(), ddsName, modelName))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Model From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_LightShader = new LightShaderClass();
	if (!m_LightShader->Init(DXEngine->GetDevice(), hWnd))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init LightShader From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_ModelList = new ModelList();
	if (!m_ModelList->Init(25))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create ModelList From FrustumTestScene", L"Error", MB_OK);
#endif
		return false;
	}

	m_Frustum = new FrustumCulling();

	m_Light = new Light();

	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(1000.0f);

	m_FrustumSceneText = new Text();
	if (!m_FrustumSceneText->Init(D2DEngine, (WCHAR*)L""))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init Text From FrustumScene", L"Error", MB_OK);
#endif
		return false;
	}

	return true;
}

bool FrustumTestScene::Render(DirectXEngine* DXEngine)
{
	float positionX = 0.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;
	float radius = 1.0f;

	XMFLOAT4 color;

	// 카메라의 위치에 따라 뷰 행렬을 생성
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투형 행렬을 가져옴
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	DXEngine->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	DXEngine->GetProjectionMatrix(projectionMatrix);
	DXEngine->GetOrthoMatrix(orthoMatrix);

	m_Frustum->InitFrustum(DXEngine->GetScreenDepth(), projectionMatrix, viewMatrix);

	int modelCount = m_ModelList->GetModelCount();

	int renderCount = 0;

	for (int index = 0; index < modelCount; index++)
	{
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		if (m_Frustum->CheckSphere(positionX, positionY, positionZ, radius))
		{
			worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);
			worldMatrix = worldMatrix * XMMatrixScaling(0.1f, 0.01f, 0.1f);
			m_Model->Render(DXEngine->GetDeviceContext());

			m_LightShader->Render(DXEngine->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
				projectionMatrix, m_Model->GetTexture(), m_Light->GetAmbientColor(), m_Light->GetDirection(), color);

			DXEngine->GetWorldMatrix(worldMatrix);

			renderCount++;
		}
	}

	DXEngine->TurnZBufferOff();

	WCHAR FrustumString[32];
	swprintf(FrustumString, L"렌더링 된 오브젝트 수 : %d", renderCount);
	m_FrustumSceneText->DrawDirect(2.0f, 45.0f, FrustumString);

	DXEngine->TurnZBufferOn();

	return true;
}

bool FrustumTestScene::Update(DirectXEngine* DXEngine)
{
	return true;
}

void FrustumTestScene::Quit()
{
	if (m_FrustumSceneText)
	{
		m_FrustumSceneText->Quit();
		delete m_FrustumSceneText;
	}

	if (m_Light)
	{
		delete m_Light;
	}

	if (m_Frustum)
	{
		delete m_Frustum;
	}

	if (m_ModelList)
	{
		m_ModelList->Quit();
		delete m_ModelList;
	}

	if (m_LightShader)
	{
		m_LightShader->Quit();
		delete m_LightShader;
	}

	if (m_Model)
	{
		m_Model->Quit();
		delete m_Model;
	}

	if (m_CameraPosition)
	{
		delete m_CameraPosition;
	}

	if (m_Camera)
	{
		m_Camera->Quit();
		delete m_Camera;
	}
}

bool FrustumTestScene::MouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool FrustumTestScene::KeyBoardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	m_CameraPosition->SetFrameTime(m_ParentTimer->GetTime());

	if (m_ParentTimer != NULL)
	{
		switch (message)
		{
			case WM_KEYDOWN:
			{
				switch (wParam)
				{
				case 'A':
				case 'a':
					m_CameraPosition->TurnLeft(true);
					break;

				case 'D':
				case 'd':
					m_CameraPosition->TurnRight(true);
					break;
				}
				float rotationY = 0.0f;
				m_CameraPosition->GetRotation(rotationY);

				m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
				m_Camera->SetRotation(0.0f, rotationY, 0.0f);
			}
		}
	}
	else
	{

	}

	return true;
}