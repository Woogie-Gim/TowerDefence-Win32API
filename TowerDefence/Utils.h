#pragma once
#include <windows.h>
#include <string>

using namespace std;

/// GDI ��� ����, �ؽ�Ʈ ��� ��ƿ��Ƽ �Լ� ����
class Utils
{
public:
    // �ؽ�Ʈ ���
    static void DrawText(HDC hdc, Pos pos, const wstring& str);

    // �߽� ��ǥ ���� �簢�� �׸���
    static void DrawRect(HDC hdc, Pos pos, int32 w, int32 h);

    // �߽� ��ǥ ���� �� �׸���
    static void DrawCircle(HDC hdc, Pos pos, int32 radius);

    // �� �� ���� �� �׸���
    static void DrawLine(HDC hdc, Pos from, Pos to);
};
