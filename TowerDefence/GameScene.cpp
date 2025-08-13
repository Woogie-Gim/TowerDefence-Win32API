#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "TimeManager.h"

extern std::wstring MakeAssetPath(const std::wstring& rel);

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Init()
{
    // 맵 초기화
    _map.Init();

	// 스포너 설정
    _spawner.SetMap(&_map);      // 경로 전달
    _spawner.SetInterval(1.5f);  // 1.5 초 마다 스폰

	//Player* player = GET_SINGLE(ObjectManager)->CreateObject<Player>();
	//GET_SINGLE(ObjectManager)->Add(player);

    _bgm.SetSource(MakeAssetPath(L"Assets\\BGM\\BGM.mp3")); // mp3 OK
    _bgm.PlayLoop();                                        // 50% 볼륨, 루프 재생
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
    // 맵 먼저 그리기
    _map.Render(hdc);

    // 오브젝트 렌더
	const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();

	for (Object* object : objects)
	{
		object->Render(hdc);
	}
}