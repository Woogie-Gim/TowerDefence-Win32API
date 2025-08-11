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
	// ������ ����
	_spawner.SetInterval(0.8f);      // 0.8�ʸ��� ����
	_spawner.SetSpawnX(-50.f);       // ȭ�� ���� �ۿ��� ����
	_spawner.SetSpawnY(150.f, 450.f);

	Player* player = GET_SINGLE(ObjectManager)->CreateObject<Player>();
	GET_SINGLE(ObjectManager)->Add(player);
}

void GameScene::Update()
{
    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

    // ���� ������ Ʈ����
    _spawner.Update(deltaTime);

    // ������Ʈ ������Ʈ (��ȸ �� ���� ����)
    const auto snapshot = GET_SINGLE(ObjectManager)->GetObjects();
    const auto& live = GET_SINGLE(ObjectManager)->GetObjects();

    for (Object* object : snapshot)
    {
        if (!object) continue;
        if (std::find(live.begin(), live.end(), object) == live.end())
            continue; // �̹� ���ŵ� ��ü�� ��ŵ
        object->Update();
    }

    // ������ �������� ���� ��� ��ü ����
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