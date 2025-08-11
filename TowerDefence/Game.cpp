#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"

Game::Game()
{
    // ������
}

Game::~Game()
{
    // �Ҹ���
    GET_SINGLE(SceneManager)->Clear();

    _CrtDumpMemoryLeaks();
}

void Game::Init(HWND hwnd)
{
    _hwnd = hwnd;

    // ������ Ŭ���̾�Ʈ ������ DC�� ������
    _hdc = ::GetDC(hwnd);

    // Ŭ���̾�Ʈ ���� ũ�� ���
    ::GetClientRect(hwnd, &_rect);

    // �� ���ۿ� DC ���� (���� ���۸�)
    _hdcBack = CreateCompatibleDC(_hdc);

    // �� ���ۿ� ��Ʈ�� ���� (ȭ��� ���� ũ��)
    _bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom);

    // �� ���� DC�� ��Ʈ�� ����
    HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack);

    // ������ ����� ��Ʈ�� ����
    ::DeleteObject(prev);

    // �̱��� �Ŵ��� �ʱ�ȭ
    GET_SINGLE(TimeManager)->Init();
    GET_SINGLE(InputManager)->Init(hwnd);
    GET_SINGLE(SceneManager)->Init();

    // Default Scene
    GET_SINGLE(SceneManager)->ChangeScene(SceneType::GameScene);
}

void Game::Update()
{
    // �ð�, �Է� �Ŵ��� ������Ʈ
    GET_SINGLE(TimeManager)->Update();
    GET_SINGLE(InputManager)->Update();
    GET_SINGLE(SceneManager)->Update();
}

void Game::Render()
{
    uint32 fps = GET_SINGLE(TimeManager)->GetFps();
    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

    {
        // ���콺 ��ġ ���ڿ� ���
        POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
        wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
        ::TextOut(_hdcBack, 20, 10, str.c_str(), static_cast<int32>(str.size()));
    }


    {
        // FPS�� DeltaTime ���ڿ� ���
        wstring str = std::format(L"FPS({0}), DT({1} ms)", fps, static_cast<int32>(deltaTime * 1000));
        ::TextOut(_hdcBack, 650, 10, str.c_str(), static_cast<int32>(str.size()));
    }

    GET_SINGLE(SceneManager)->Render(_hdcBack);

    // Double Buffering

    // �� ����(_hdcBack)�� �׷��� ������ ����Ʈ ����(_hdc)�� ����
    ::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY);

    // �� ���� �ʱ�ȭ (������� ����)
    ::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
