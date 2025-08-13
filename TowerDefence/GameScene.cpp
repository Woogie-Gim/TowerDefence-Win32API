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
    // �� �ʱ�ȭ
    _map.Init();

	// ������ ����
    _spawner.SetMap(&_map);      // ��� ����
    _spawner.SetInterval(1.5f);  // 1.5 �� ���� ����

	//Player* player = GET_SINGLE(ObjectManager)->CreateObject<Player>();
	//GET_SINGLE(ObjectManager)->Add(player);

    _bgm.SetSource(MakeAssetPath(L"Assets\\BGM\\BGM.mp3")); // mp3 OK
    _bgm.PlayLoop();                                        // 50% ����, ���� ���
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
    // �� ���� �׸���
    _map.Render(hdc);

    // ������Ʈ ����
	const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();

	for (Object* object : objects)
	{
		object->Render(hdc);
	}
}