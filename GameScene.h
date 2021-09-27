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
// 매크로

/*
GameScene : 모든 게임의 장면을 관리한다.
이 게임은 장면, 장면으로 분할 시켜서 각종 이벤트들을 관리한다.
예를 들어 게임을 불러오기 전에 초기화 장면, 메인 메뉴들을 보여주는 장면, 실제 게임 장면등등
세부적인 장면으로 잘게 쪼개어서 이를 관리한다.

필수적인 기능은 장면이 전환될 때 자동으로 생성되었던 장면을 릴리즈 하는 것이다.

Hint : 백그라운드는 메인 메뉴에서 공유할 수 있도록 한다.
*/

class GameScene
{
private:
	SceneStatus m_SceneStatus;
	bool IsChangeScene; // 장면이 전환되면 트루로 설정한다. (Release를 하기 위함)
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
	// 기본적인 씬의 초기화
	bool Init();
	bool Init(DirectXEngine* dxEngine, Direct2DEngine* dx2dEngine, HWND hWnd, int width, int height);
	// 렌더링할 모델링을 적재하는 곳이다.
	bool PreRender(); // 렌더 텍스처 용도로 사용.

	bool Render();

	bool AfterRender();

	// 업데이트 보통 메시지를 처리한다.
	bool Update();

	// 게임이 종료되거나 장면이 전환될 때 사용한다.
	bool Release();

	// 게임의 장면에 따른 이벤트 처리
	bool MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool KeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif