#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "TimeManager.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Init()
{
	// 스포너 설정
	_spawner.SetInterval(0.8f);      // 0.8초마다 스폰
	_spawner.SetSpawnX(-50.f);       // 화면 왼쪽 밖에서 등장
	_spawner.SetSpawnY(150.f, 450.f);

	Player* player = GET_SINGLE(ObjectManager)->CreateObject<Player>();
	GET_SINGLE(ObjectManager)->Add(player);
}

void GameScene::Update()
{
    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

    // 무한 스포너 트리거
    _spawner.Update(deltaTime);

    // 오브젝트 업데이트 (순회 중 삭제 방지)
    const auto snapshot = GET_SINGLE(ObjectManager)->GetObjects();
    const auto& live = GET_SINGLE(ObjectManager)->GetObjects();

    for (Object* object : snapshot)
    {
        if (!object) continue;
        if (std::find(live.begin(), live.end(), object) == live.end())
            continue; // 이미 제거된 객체면 스킵
        object->Update();
    }

    // 프레임 마지막에 제거 대기 객체 정리
    GET_SINGLE(ObjectManager)->CollectGarbage();
}

void GameScene::Render(HDC hdc)
{
	const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();

	for (Object* object : objects)
	{
		object->Render(hdc);
	}
}