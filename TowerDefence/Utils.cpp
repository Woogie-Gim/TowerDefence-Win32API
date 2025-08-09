#include "pch.h"
#include "Utils.h"

// 지정된 위치에 유니코드 문자열 출력
void Utils::DrawText(HDC hdc, Pos pos, const wstring& str)
{
    ::TextOut(hdc, static_cast<int32>(pos.x), static_cast<int32>(pos.y), str.c_str(), static_cast<int32>(str.size()));
}

// 중심 좌표를 기준으로 사각형 그리기
void Utils::DrawRect(HDC hdc, Pos pos, int32 w, int32 h)
{
    ::Rectangle(hdc,
        static_cast<int32>(pos.x - w / 2), static_cast<int32>(pos.y - h / 2),
        static_cast<int32>(pos.x + w / 2), static_cast<int32>(pos.y + h / 2));
}

// 중심 좌표를 기준으로 원 그리기
void Utils::DrawCircle(HDC hdc, Pos pos, int32 radius)
{
    ::Ellipse(hdc,
        static_cast<int32>(pos.x - radius), static_cast<int32>(pos.y - radius),
        static_cast<int32>(pos.x + radius), static_cast<int32>(pos.y + radius));
}

// 시작점에서 끝점까지 직선 그리기
void Utils::DrawLine(HDC hdc, Pos from, Pos to)
{
    ::MoveToEx(hdc, static_cast<int32>(from.x), static_cast<int32>(from.y), nullptr);
    ::LineTo(hdc, static_cast<int32>(to.x), static_cast<int32>(to.y));
}
