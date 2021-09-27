#pragma once
#ifndef _FRUSTUMTESTSCENE_H_
#define _FRUSTUMTESTSCENE_H_

#include "WindowOption.h"
#include "Scene.h"
#include "DirectXEngine.h"
#include "Direct2DEngine.h"
#include "Camera.h"
#include "ModelObject.h"
#include "ModelList.h"
#include "Light.h"
#include "LightShaderClass.h"
#include "FrustumCulling.h"
#include "CameraPosition.h"
#include "Text.h"
#include "SystemManager.h"

class FrustumTestScene : public Scene
{
private:
	Camera* m_Camera = nullptr;
	CameraPosition* m_CameraPosition = nullptr;

	ModelObject* m_Model = nullptr;
	ModelList* m_ModelList = nullptr;
	FrustumCulling* m_Frustum = nullptr;
	Light* m_Light = nullptr;
	LightShaderClass* m_LightShader = nullptr;

	Text* m_FrustumSceneText = nullptr;

	// 상위 클래스에서 얻은 포인터
	Timer* m_ParentTimer;
public:
	FrustumTestScene();
	FrustumTestScene(const FrustumTestScene& other);
	~FrustumTestScene();

	bool Init(DirectXEngine*, Direct2DEngine*, HWND, int, int);
	bool Init(DirectXEngine*, Direct2DEngine*, HWND, int, int, Timer*);
	bool Render(DirectXEngine*);
	bool Update(DirectXEngine*);
	void Quit();

	bool MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool KeyBoardEvent(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif
