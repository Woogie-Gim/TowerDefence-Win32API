#include "pch.h"
#include "SceneManager.h"
#include "GameScene.h"

void SceneManager::Init()
{

}

// 현재 Scene의 업데이트 함수 호출
void SceneManager::Update()
{
	if (_scene)
	{
		_scene->Update();
	}
}

// 현재 Scene의 렌더링 함수 호출
void SceneManager::Render(HDC hdc)
{
	if (_scene)
	{
		_scene->Render(hdc);
	}
}

// 현재 Scene 삭제
void SceneManager::Clear()
{
	SAFE_DELETE(_scene);
}

// Scene 전환 함수
void SceneManager::ChangeScene(SceneType sceneType)
{
	// 이미 동일한 Scene이면 변경하지 않음
	if (_sceneType == sceneType)
	{
		return;
	}

	Scene* newScene = nullptr;

	// Scene 타입에 따라 새로운 Scene 인스턴스를 생성
	switch (sceneType)
	{
	case SceneType::GameScene:
		newScene = new GameScene();
		break;
	}

	// 기존 Scene 삭제
	SAFE_DELETE(_scene);

	// 새 Scene으로 교체
	_scene = newScene;
	_sceneType = sceneType;

	// 새 Scene 초기화
	newScene->Init();
}
