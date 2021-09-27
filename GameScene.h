#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_
#include "SceneStatus.h"
#include "TestScene.h"
#include "TestMulitTextureScene.h"

#include "Direct2DEngine.h"
#include "DirectXEngine.h"
#include "SystemManager.h"

#include "Text.h"
#include "CEllipse.h"
#include "CBitmap.h"
// ��ũ��

/*
GameScene : ��� ������ ����� �����Ѵ�.
�� ������ ���, ������� ���� ���Ѽ� ���� �̺�Ʈ���� �����Ѵ�.
���� ��� ������ �ҷ����� ���� �ʱ�ȭ ���, ���� �޴����� �����ִ� ���, ���� ���� �����
�������� ������� �߰� �ɰ�� �̸� �����Ѵ�.

�ʼ����� ����� ����� ��ȯ�� �� �ڵ����� �����Ǿ��� ����� ������ �ϴ� ���̴�.

Hint : ��׶���� ���� �޴����� ������ �� �ֵ��� �Ѵ�.
*/

class GameScene
{
private:
	SceneStatus m_SceneStatus;
	bool IsChangeScene; // ����� ��ȯ�Ǹ� Ʈ��� �����Ѵ�. (Release�� �ϱ� ����)
	TestScene* m_TestScene;
	TestMulitTextureScene* m_TestMultiTextureScene;
	// DirectX
	DirectXEngine* m_dxEngine;
	Direct2DEngine* m_2dEngine;

	// System
	SystemManager m_SystemManager;

	// Direct2D
	Text* m_SystemText;
	CEllipse* m_Ellipse;
	CBitmap* m_Bitmap;

	WCHAR m_FpsText[32];
	WCHAR m_CpuText[32];
public:
	// �⺻���� ���� �ʱ�ȭ
	bool Init();
	bool Init(DirectXEngine* dxEngine, Direct2DEngine* dx2dEngine, HWND hWnd, int width, int height);
	// �������� �𵨸��� �����ϴ� ���̴�.
	bool PreRender(); // ���� �ؽ�ó �뵵�� ���.

	bool Render();

	bool AfterRender();

	// ������Ʈ ���� �޽����� ó���Ѵ�.
	bool Update();

	// ������ ����ǰų� ����� ��ȯ�� �� ����Ѵ�.
	bool Release();

	// ������ ��鿡 ���� �̺�Ʈ ó��
	bool MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool KeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif