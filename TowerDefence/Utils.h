#pragma once
#include <windows.h>
#include <string>

using namespace std;

/// GDI 기반 도형, 텍스트 출력 유틸리티 함수 모음
class Utils
{
public:
    // 텍스트 출력
    static void DrawText(HDC hdc, Pos pos, const wstring& str);

    // 중심 좌표 기준 사각형 그리기
    static void DrawRect(HDC hdc, Pos pos, int32 w, int32 h);

    // 중심 좌표 기준 원 그리기
    static void DrawCircle(HDC hdc, Pos pos, int32 radius);

    // 두 점 사이 선 그리기
    static void DrawLine(HDC hdc, Pos from, Pos to);
};
