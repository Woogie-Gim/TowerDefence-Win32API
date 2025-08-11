#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"

Game::Game()
{
    // 생성자
}

Game::~Game()
{
    // 소멸자
    GET_SINGLE(SceneManager)->Clear();

    _CrtDumpMemoryLeaks();
}

void Game::Init(HWND hwnd)
{
    _hwnd = hwnd;

    // 윈도우 클라이언트 영역의 DC를 가져옴
    _hdc = ::GetDC(hwnd);

    // 클라이언트 영역 크기 계산
    ::GetClientRect(hwnd, &_rect);

    // 백 버퍼용 DC 생성 (더블 버퍼링)
    _hdcBack = CreateCompatibleDC(_hdc);

    // 백 버퍼용 비트맵 생성 (화면과 같은 크기)
    _bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom);

    // 백 버퍼 DC에 비트맵 연결
    HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack);

    // 이전에 연결된 비트맵 삭제
    ::DeleteObject(prev);

    // 싱글톤 매니저 초기화
    GET_SINGLE(TimeManager)->Init();
    GET_SINGLE(InputManager)->Init(hwnd);
    GET_SINGLE(SceneManager)->Init();

    // Default Scene
    GET_SINGLE(SceneManager)->ChangeScene(SceneType::GameScene);
}

void Game::Update()
{
    // 시간, 입력 매니저 업데이트
    GET_SINGLE(TimeManager)->Update();
    GET_SINGLE(InputManager)->Update();
    GET_SINGLE(SceneManager)->Update();
}

void Game::Render()
{
    uint32 fps = GET_SINGLE(TimeManager)->GetFps();
    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

    {
        // 마우스 위치 문자열 출력
        POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
        wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
        ::TextOut(_hdcBack, 20, 10, str.c_str(), static_cast<int32>(str.size()));
    }


    {
        // FPS와 DeltaTime 문자열 출력
        wstring str = std::format(L"FPS({0}), DT({1} ms)", fps, static_cast<int32>(deltaTime * 1000));
        ::TextOut(_hdcBack, 650, 10, str.c_str(), static_cast<int32>(str.size()));
    }

    GET_SINGLE(SceneManager)->Render(_hdcBack);

    // Double Buffering

    // 백 버퍼(_hdcBack)에 그려진 내용을 프론트 버퍼(_hdc)로 복사
    ::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY);

    // 백 버퍼 초기화 (흰색으로 덮기)
    ::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
