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
    // 맵 초기화
    _map.Init();

	// 스포너 설정
    _spawner.SetMap(&_map);      // 경로 전달
    _spawner.SetInterval(2.5f);  // 2.5 초 마다 스폰

	Player* player = GET_SINGLE(ObjectManager)->CreateObject<Player>();
	GET_SINGLE(ObjectManager)->Add(player);

    Player1* player1 = GET_SINGLE(ObjectManager)->CreateObject<Player1>();
    GET_SINGLE(ObjectManager)->Add(player1);

    _bgm.SetSource(MakeAssetPath(L"Assets\\BGM\\BGM.mp3")); // mp3 OK
    _bgm.PlayLoop();                                        // 50% 볼륨, 루프 재생

    // 라이프 20
    _lifeUI.Init({ 10,10 }, 20);
    _gameOver = false;

    // 적 탈출 시 라이프 1 감소 → 0이면 게임오버 + 카운트다운 시작
    _spawner.SetOnEnemyEscaped([this]()
    {
        if (_gameOver) return;
        _lifeUI.Sub(1);
        if (_lifeUI.IsDead()) {
            _gameOver = true;
            _restartCountdown = 5.0f; // ★ 5초 후 자동 재시작
        }
    });

    InitWorld();
}

void GameScene::Update()
{
    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();


    if (_gameOver)
    {
        // 카운트다운 진행
        if (_restartCountdown > 0.0f) 
        {
            _restartCountdown -= deltaTime;
            if (_restartCountdown <= 0.0f) 
            {
                RestartGame();
            }
        }
        // 게임오버 상태에서는 나머지 게임 로직은 멈춤
        return;
    }

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

    _lifeUI.Render(hdc);

    if (_gameOver)
    {
        RECT rc; GetClientRect(GetActiveWindow(), &rc);

        // 오버레이(간단히 어둡게)
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

        // ★ RESTART IN N
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
    // 모든 오브젝트 제거
    GET_SINGLE(ObjectManager)->ClearAll();

    // 맵/스포너 재초기화
    _map.Init();
    _spawner.Reset();
    _spawner.SetMap(&_map);
    _spawner.SetInterval(1.5f);

    // 라이프/상태 재초기화
    _lifeUI.Init({ 10,10 }, 20);
    _gameOver = false;
    _restartCountdown = 0.f;

    // 적 탈출 시 라이프 감소 → 0이면 게임오버 + 카운트다운
    _spawner.SetOnEnemyEscaped([this]() 
    {
        if (_gameOver) return;

        _lifeUI.Sub(1);

        if (_lifeUI.IsDead()) 
        {
            _gameOver = true;
            _restartCountdown = 5.0f; // 자동 재시작 카운트다운(원하면 0으로)
        }
    });
}
