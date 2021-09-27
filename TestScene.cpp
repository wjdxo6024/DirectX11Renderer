#include "TestScene.h"

bool TestScene::Init(DirectXEngine* DXEngine, Direct2DEngine* D2DEngine, HWND hWnd, int width, int height)
{
	// ���콺 ��ǥ --------------
	x = y = 0;
	swprintf(m_xText, L"���콺 ��ǥ X : %d", x);
	swprintf(m_yText, L"���콺 ��ǥ Y : %d", y);
	// Ű���� -------------------
	swprintf(m_KeyboardText, L"Ű���� ���� : ");

	// --------------------------

	m_Camera = new Camera();

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// ����Ʈ

	m_Model = new ModelObject();
	WCHAR ddsName[] = L"./Texture/Red.dds";
	char modelName[] = "./Model/humanoid.fbx";
	if (!m_Model->Init(DXEngine->GetDevice(), DXEngine->GetDeviceContext(), ddsName, modelName))
	{
		MessageBox(NULL, L"Cannot Init Model.", L"Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShaderClass();
	if (!m_TextureShader->Init(DXEngine->GetDevice(), hWnd))
	{
		MessageBox(NULL, L"Cannot Init Texture Shader.", L"Error", MB_OK);
		return false;
	}

	m_LightShader = new LightShaderClass();
	if (!m_LightShader->Init(DXEngine->GetDevice(), hWnd))
	{
		MessageBox(NULL, L"Cannot Init Light Shader.", L"Error", MB_OK);
		return false;
	}

	m_PhongShader = new PhongShaderClass();
	if (!m_PhongShader->Init(DXEngine->GetDevice(), hWnd))
	{
		MessageBox(NULL, L"Cannot Init Phong Shader.", L"Error", MB_OK);
		return false;
	}

	WCHAR SpritefileName[] = L"./Texture/1_store.dds";
	m_TestSprite = new Sprite();
	if (!m_TestSprite->Init(DXEngine->GetDevice(), DXEngine->GetDeviceContext(), width, height,
		SpritefileName, 256, 256))
	{
		MessageBox(NULL, L"Cannot Init Sprite.", L"Error", MB_OK);
		return false;
	}

	m_Light = new Light();

	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(1000.0f);

	// TestText
	m_TestText = new Text();
	WCHAR TestString[] = L"�̰��� �׽�Ʈ ���ڿ��Դϴ�. LotEngine Make By Lot626";
	if (!m_TestText->Init(D2DEngine, TestString))
	{
		return false;
	}
	return true;
}

bool TestScene::Render(DirectXEngine* DXEngine)
{
	// ī�޶��� ��ġ�� ���� �� ����� ����
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� ������
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	DXEngine->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	DXEngine->GetProjectionMatrix(projectionMatrix);
	DXEngine->GetOrthoMatrix(orthoMatrix);

	worldMatrix = XMMatrixRotationY(rotation);
	worldMatrix = worldMatrix * XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(0.0f));
	m_Model->Render(DXEngine->GetDeviceContext());

	if (!m_LightShader->Render(DXEngine->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_Model->GetTexture(), m_Light->GetAmbientColor(), m_Light->GetDirection(), m_Light->GetDiffuseColor()))
	{
		return false;
	}

	//if (!m_PhongShader->Render(DXEngine->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
	//	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
	//{
	//	return false;
	//}

	// ��������Ʈ�� ���� ZBuffer Off
	DXEngine->TurnZBufferOff();

	if (!m_TestSprite->Render(DXEngine->GetDeviceContext(), 50, 50))
	{
		return false;
	}

	// sprite�� �ؽ���
	//if (!m_TextureShader->Render(DXEngine->GetDeviceContext(), m_TestSprite->GetIndexCount(), XMMatrixIdentity(),
	//	viewMatrix, orthoMatrix, m_TestSprite->GetTexture()))
	//{
	//	return false;
	//}

	DXEngine->TurnZBufferOn();

	// D2D ������
	//m_TestText->Draw(2.0f, 10.0f);
	//m_TestText->DrawDirect(2.0f, 30.0f, m_xText);
	//m_TestText->DrawDirect(2.0f, 50.0f, m_yText);
	//m_TestText->DrawDirect(2.0f, 70.0f, m_KeyboardText);
	//m_TestText->DrawDirect(52.0f, 70.0f, (WCHAR*)wide_string.c_str());
	return true;
}

bool TestScene::Update(DirectXEngine* DXEngine)
{
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// ���콺 ��ǥ
	swprintf(m_xText, L"���콺 ��ǥ X : %d", x);
	swprintf(m_yText, L"���콺 ��ǥ Y : %d", y);
	
	wide_string.assign(str.begin(), str.end());
	wide_string.push_back('\0');

	return true;
}

void TestScene::Quit()
{
	if (m_TestText)
	{
		delete m_TestText;
		m_TestText = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_TestSprite)
	{
		m_TestSprite->Quit();
		delete m_TestSprite;
		m_TestSprite = 0;
	}
	
	if (m_PhongShader)
	{
		m_PhongShader->Quit();
		delete m_PhongShader;
		m_PhongShader = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Quit();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_Model)
	{
		m_Model->Quit();
		delete m_Model;
		m_Model = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Quit();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_ColorShader)
	{
		m_ColorShader->Quit();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_Triangle)
	{
		m_Triangle->Quit();
		delete m_Triangle;
		m_Triangle = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
}

void TestScene::GetXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void TestScene::GetInput(char input)
{
	if (input == 0)
	{
		return;
	}
	
	str.push_back(input);
}

bool TestScene::MouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	return true;
}

bool TestScene::KeyBoardEvent(UINT message, WPARAM wParam, LPARAM lParam)
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