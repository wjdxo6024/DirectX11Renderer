#pragma once
#ifndef _MULTITEXTURESCENE_H_
#define _MULTITEXTURESCENE_H_
#include "ModelScene.h"
#include "ModelShaders.h"
#include "TestBumpMapObject.h"

#include "WindowOption.h"
#include "Scene.h"
#include "DirectXEngine.h"
#include "Direct2DEngine.h"
#include "Camera.h"
#include "ModelObject.h"
#include "ModelList.h"
#include "Light.h"
#include "CameraPosition.h"
#include "Text.h"
#include "SystemManager.h"

#include "RenderTexture.h"
#include "RenderTargetDrawer.h"

class TestMulitTextureScene
{
private:
	Camera* m_Camera = nullptr;
	CameraPosition* m_CameraPosition = nullptr;

	ModelList* m_ModelList = nullptr;
	ModelScene* m_ModelScene = nullptr;
	ModelShaders* m_ModelShader = nullptr;
	Light* m_Light = nullptr;

	// RTT -----------------------------
	RenderTexture* m_RenderTexture;
	RenderTargetDrawer* m_RenderTextureDrawer;
	// ---------------------------------

	Text* m_FrustumSceneText = nullptr;

	float rotation = 0.0f;
	// 상위 클래스에서 얻은 포인터
	Timer* m_ParentTimer;

private:
	bool RenderScene(DirectXEngine* DXEngine);

public:
	TestMulitTextureScene();
	TestMulitTextureScene(const TestMulitTextureScene& other);
	~TestMulitTextureScene();

	bool Init(DirectXEngine*, Direct2DEngine*, HWND, int, int);
	bool Init(DirectXEngine*, Direct2DEngine*, HWND, int, int, Timer*);

	bool PreRender(DirectXEngine* DXEngine); // 렌더 텍스쳐
	bool Render(DirectXEngine*);
	bool AfterRender(DirectXEngine* DXEngine);

	bool Update(DirectXEngine*);

	void Quit();

	bool MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool KeyBoardEvent(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif