#pragma once
/*
SceneStatus : 게임에서 보여줄 여러 장면들을 번호로 관리한다.
*/

enum GameSceneStatusNumber {
	// 게임 실행시 초기화 할때 보여줄 장면
	WaitGameInitScene = -1,

	// 메인 메뉴 0~10
	MainMenuScene = 0,

	// 옵션

	// 게임 종료

	// 시나리오
	ScenarioMenuScene = 1, // 시나리오 - 
	ScenarioLoadScene = 2,

	// 싱글 게임 (시나리오가 아닌 빠른 시작)
};

class SceneStatus
{
private:

public:
	bool Init();
	bool ChangeScene(GameSceneStatusNumber scene);
};

