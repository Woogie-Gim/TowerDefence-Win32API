#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Player1.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "EnemySpawner.h"

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
    _spawner.SetInterval(2.5f);  // 2.5 �� ���� ����

	Player* player = GET_SINGLE(ObjectManager)->CreateObject<Player>();
	GET_SINGLE(ObjectManager)->Add(player);

    Player1* player1 = GET_SINGLE(ObjectManager)->CreateObject<Player1>();
    GET_SINGLE(ObjectManager)->Add(player1);

    _bgm.SetSource(MakeAssetPath(L"Assets\\BGM\\BGM.mp3")); // mp3 OK
    _bgm.PlayLoop();                                        // 50% ����, ���� ���

    // ������ 20
    _lifeUI.Init({ 10,10 }, 20);
    _gameOver = false;

    // �� Ż�� �� ������ 1 ���� �� 0�̸� ���ӿ��� + ī��Ʈ�ٿ� ����
    _spawner.SetOnEnemyEscaped([this]()
    {
        if (_gameOver) return;
        _lifeUI.Sub(1);
        if (_lifeUI.IsDead()) {
            _gameOver = true;
            _restartCountdown = 5.0f; // �� 5�� �� �ڵ� �����
        }
    });

    InitWorld();
}

void GameScene::Update()
{
    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();


    if (_gameOver)
    {
        // ī��Ʈ�ٿ� ����
        if (_restartCountdown > 0.0f) 
        {
            _restartCountdown -= deltaTime;
            if (_restartCountdown <= 0.0f) 
            {
                RestartGame();
            }
        }
        // ���ӿ��� ���¿����� ������ ���� ������ ����
        return;
    }

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

    _lifeUI.Render(hdc);

    if (_gameOver)
    {
        RECT rc; GetClientRect(GetActiveWindow(), &rc);

        // ��������(������ ��Ӱ�)
        HBRUSH dim = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &rc, dim);
        DeleteObject(dim);

        SetBkMode(hdc, TRANSPARENT);

        // GAME OVER
        SetTextColor(hdc, RGB(255, 80, 80));
        const wchar_t* overTxt = L"GAME OVER";
        SIZE sz{};
        GetTextExtentPoint32W(hdc, overTxt, lstrlenW(overTxt), &sz);
        int cx = (rc.right - rc.left) / 2;
        int cy = (rc.bottom - rc.top) / 2 - 40;
        TextOutW(hdc, cx - sz.cx / 2, cy - sz.cy / 2, overTxt, lstrlenW(overTxt));

        // �� RESTART IN N
        int remain = (int)ceilf((_restartCountdown > 0.f) ? _restartCountdown : 0.f);
        wchar_t buf[64];
        wsprintfW(buf, L"RESTART IN %d", remain);
        SetTextColor(hdc, RGB(220, 220, 220));
        SIZE sz2{};
        GetTextExtentPoint32W(hdc, buf, lstrlenW(buf), &sz2);
        TextOutW(hdc, cx - sz2.cx / 2, cy - sz2.cy / 2 + 40, buf, lstrlenW(buf));
    }

	for (Object* object : objects)
	{
		object->Render(hdc);
	}
}

void GameScene::OnLButtonDown(int mx, int my)
{
    if (_gameOver)
        return;
}

void GameScene::RestartGame()
{
    InitWorld();

     _bgm.SetSource(MakeAssetPath(L"Assets\\BGM\\BGM.mp3"));
     _bgm.PlayLoop();
}

void GameScene::InitWorld()
{
    // ��� ������Ʈ ����
    GET_SINGLE(ObjectManager)->ClearAll();

    // ��/������ ���ʱ�ȭ
    _map.Init();
    _spawner.Reset();
    _spawner.SetMap(&_map);
    _spawner.SetInterval(1.5f);

    // ������/���� ���ʱ�ȭ
    _lifeUI.Init({ 10,10 }, 20);
    _gameOver = false;
    _restartCountdown = 0.f;

    // �� Ż�� �� ������ ���� �� 0�̸� ���ӿ��� + ī��Ʈ�ٿ�
    _spawner.SetOnEnemyEscaped([this]() 
    {
        if (_gameOver) return;

        _lifeUI.Sub(1);

        if (_lifeUI.IsDead()) 
        {
            _gameOver = true;
            _restartCountdown = 5.0f; // �ڵ� ����� ī��Ʈ�ٿ�(���ϸ� 0����)
        }
    });
}
