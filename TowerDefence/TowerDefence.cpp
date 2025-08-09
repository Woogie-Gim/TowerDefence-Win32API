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

    // 1) ������ â ���� ���
    MyRegisterClass(hInstance);

    // 2) ������ â ����
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    Game game;
    game.Init(g_hWnd);

    MSG msg = {};
    uint64 prevTick = 0; // ���� ������ ���� ���� �����

    // ���� ���� ����
    // �޽��� ó���� ���� ������ ���ÿ� �ٷ�� ����
    while (msg.message != WM_QUIT)
    {
        // ������ �޽����� ���� ��� ó��
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);   // Ű �Է� �� �޽��� ����
            ::DispatchMessage(&msg);    // WndProc���� �޽��� ����
        }
        else
        {
            // �ý��� �޽����� ���� �� ���� ���� ����
            uint64 now = ::GetTickCount64(); // ���� �ð� (�и��� ����)

            // ���� ���������κ��� 10ms �̻� �����ٸ� ������ ����
            if (now - prevTick >= 10)
            {
                // ���� ���� ������Ʈ (�Է�, ������, �浹 ��)
                game.Update();
                // ȭ�� ������
                game.Render();
                // �̹� ������ �ð� ���
                prevTick = now;
            }
        }
    }

    return (int)msg.wParam; // ���α׷� ���� �ڵ� ��ȯ
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


// ���ø����̼��� ���� �����츦 �����ϰ� ȭ�鿡 ǥ���ϴ� �Լ�
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // ���ø����̼� �ν��Ͻ� �ڵ��� ���� ������ ����
    hInst = hInstance;
    // ���ϴ� Ŭ���̾�Ʈ ���� ũ�� ���� (800x600)
    RECT windowRect = { 0, 0, 800, 600 };
    // ���� â ũ�⸦ ��� (Ÿ��Ʋ ��, �׵θ� ����)
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    // ���� ������ ����
    HWND hWnd = CreateWindowW(
        L"TowerDefence",             // ������ Ŭ���� �̸�
        L"Cat Tower",                // â Ÿ��Ʋ
        WS_OVERLAPPEDWINDOW,         // â ��Ÿ�� (Ÿ��Ʋ�� + �ݱ� ��ư �� ����)
        CW_USEDEFAULT,               // X ��ǥ (�ڵ� ����)
        0,                           // Y ��ǥ (�ڵ� ����)
        windowRect.right - windowRect.left,   // �ʺ�
        windowRect.bottom - windowRect.top,   // ����
        nullptr,                     // �θ� â ���� (���� ������)
        nullptr,                     // �޴� ����
        hInstance,                   // �ν��Ͻ� �ڵ�
        nullptr                      // �߰� �Ķ���� ����
    );

    g_hWnd = hWnd;

    // â ���� ���� �� false ��ȯ
    if (!hWnd)
    {
        return FALSE;
    }

    // �����츦 ȭ�鿡 ǥ��
    ShowWindow(hWnd, nCmdShow);
    // Ŭ���̾�Ʈ ���� ���� (WM_PAINT ���� �߻�)
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

            // ����
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
