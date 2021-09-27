#pragma once
/*
SceneStatus : ���ӿ��� ������ ���� ������ ��ȣ�� �����Ѵ�.
*/

enum GameSceneStatusNumber {
	// ���� ����� �ʱ�ȭ �Ҷ� ������ ���
	WaitGameInitScene = -1,

	// ���� �޴� 0~10
	MainMenuScene = 0,

	// �ɼ�

	// ���� ����

	// �ó�����
	ScenarioMenuScene = 1, // �ó����� - 
	ScenarioLoadScene = 2,

	// �̱� ���� (�ó������� �ƴ� ���� ����)
};

class SceneStatus
{
private:

public:
	bool Init();
	bool ChangeScene(GameSceneStatusNumber scene);
};

