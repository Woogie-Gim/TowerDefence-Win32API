#include "pch.h"
#include "framework.h"
#include "TowerDefence.h"
#include "Game.h"

int mousePosX;
int mousePosY;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TOWERDEFENCE, szWindowClass, MAX_LOADSTRING);

    // 1) 윈도우 창 정보 등록
    MyRegisterClass(hInstance);

    // 2) 윈도우 창 생성
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    Game game;
    game.Init(g_hWnd);

    MSG msg = {};
    uint64 prevTick = 0; // 이전 프레임 갱신 시점 저장용

    // 메인 루프 시작
    // 메시지 처리와 게임 로직을 동시에 다루는 구조
    while (msg.message != WM_QUIT)
    {
        // 윈도우 메시지가 있을 경우 처리
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);   // 키 입력 등 메시지 번역
            ::DispatchMessage(&msg);    // WndProc으로 메시지 전달
        }
        else
        {
            // 시스템 메시지가 없을 때 게임 루프 실행
            uint64 now = ::GetTickCount64(); // 현재 시각 (밀리초 단위)

            // 이전 프레임으로부터 10ms 이상 지났다면 프레임 갱신
            if (now - prevTick >= 10)
            {
                // 게임 로직 업데이트 (입력, 움직임, 충돌 등)
                game.Update();
                // 화면 렌더링
                game.Render();
                // 이번 프레임 시간 기록
                prevTick = now;
            }
        }
    }

    return (int)msg.wParam; // 프로그램 종료 코드 반환
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOWERDEFENCE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = L"TowerDefence";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.


// 애플리케이션의 메인 윈도우를 생성하고 화면에 표시하는 함수
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // 애플리케이션 인스턴스 핸들을 전역 변수에 저장
    hInst = hInstance;
    // 원하는 클라이언트 영역 크기 설정 (800x600)
    RECT windowRect = { 0, 0, 800, 600 };
    // 실제 창 크기를 계산 (타이틀 바, 테두리 포함)
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    // 메인 윈도우 생성
    HWND hWnd = CreateWindowW(
        L"TowerDefence",             // 윈도우 클래스 이름
        L"Cat Tower",                // 창 타이틀
        WS_OVERLAPPEDWINDOW,         // 창 스타일 (타이틀바 + 닫기 버튼 등 포함)
        CW_USEDEFAULT,               // X 좌표 (자동 결정)
        0,                           // Y 좌표 (자동 결정)
        windowRect.right - windowRect.left,   // 너비
        windowRect.bottom - windowRect.top,   // 높이
        nullptr,                     // 부모 창 없음 (메인 윈도우)
        nullptr,                     // 메뉴 없음
        hInstance,                   // 인스턴스 핸들
        nullptr                      // 추가 파라미터 없음
    );

    g_hWnd = hWnd;

    // 창 생성 실패 시 false 반환
    if (!hWnd)
    {
        return FALSE;
    }

    // 윈도우를 화면에 표시
    ShowWindow(hWnd, nCmdShow);
    // 클라이언트 영역 갱신 (WM_PAINT 강제 발생)
    UpdateWindow(hWnd);

    return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            // 문자
            WCHAR buffer[100];
            ::wsprintf(buffer, L"(%d, %d)", mousePosX, mousePosY);
            ::TextOut(hdc, 10, 10, buffer, ::wcslen(buffer));

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_MOUSEMOVE:
        mousePosX = LOWORD(lParam);
        mousePosY = HIWORD(lParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
