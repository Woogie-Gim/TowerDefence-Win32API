#include "pch.h"
#include "SceneManager.h"
#include "GameScene.h"

void SceneManager::Init()
{

}

// ���� Scene�� ������Ʈ �Լ� ȣ��
void SceneManager::Update()
{
	if (_scene)
	{
		_scene->Update();
	}
}

// ���� Scene�� ������ �Լ� ȣ��
void SceneManager::Render(HDC hdc)
{
	if (_scene)
	{
		_scene->Render(hdc);
	}
}

// ���� Scene ����
void SceneManager::Clear()
{
	SAFE_DELETE(_scene);
}

// Scene ��ȯ �Լ�
void SceneManager::ChangeScene(SceneType sceneType)
{
	// �̹� ������ Scene�̸� �������� ����
	if (_sceneType == sceneType)
	{
		return;
	}

	Scene* newScene = nullptr;

	// Scene Ÿ�Կ� ���� ���ο� Scene �ν��Ͻ��� ����
	switch (sceneType)
	{
	case SceneType::GameScene:
		newScene = new GameScene();
		break;
	}

	// ���� Scene ����
	SAFE_DELETE(_scene);

	// �� Scene���� ��ü
	_scene = newScene;
	_sceneType = sceneType;

	// �� Scene �ʱ�ȭ
	newScene->Init();
}
