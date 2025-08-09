#pragma once
#include <cmath>
#include <Windows.h>

// 고정 폭 정수형 정의 (가독성과 플랫폼 일관성 확보)
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// 캐릭터/유닛의 기본 스탯 구조체
struct Stat
{
    int32 hp = 0;
    int32 maxHp = 0;
    float speed = 0;
};

// 2D 벡터 구조체 (기본 연산 + 수학 연산 포함)
struct Vector
{
    Vector() {}
    Vector(float x, float y) : x(x), y(y) {}
    Vector(POINT pt) : x((float)pt.x), y((float)pt.y) {}

    // 벡터 덧셈
    Vector operator+(const Vector& other)
    {
        return Vector(x + other.x, y + other.y);
    }

    // 벡터 뺄셈
    Vector operator-(const Vector& other)
    {
        return Vector(x - other.x, y - other.y);
    }

    // 스칼라 곱
    Vector operator*(float value)
    {
        return Vector(x * value, y * value);
    }

    // 복합 연산자 오버로딩
    void operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y;
    }

    void operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
    }

    void operator*=(float value)
    {
        x *= value;
        y *= value;
    }

    // 길이 제곱 (루트 없이 빠르게 거리 비교용)
    float LengthSquared()
    {
        return x * x + y * y;
    }

    // 길이 (거리 계산용)
    float Length()
    {
        return ::sqrt(LengthSquared());
    }

    // 벡터 정규화 (단위 벡터로)
    void Normalize()
    {
        float length = Length();
        if (length < 1e-10f)
            return;

        x /= length;
        y /= length;
    }

    // 내적 (Dot Product)
    float Dot(Vector other)
    {
        return x * other.x + y * other.y;
    }

    // 외적 (Cross Product)
    float Cross(Vector other)
    {
        return x * other.y - y * other.x;
    }

    float x = 0;
    float y = 0;
};

// Vector는 위치(Pos)로도 사용될 수 있으므로 별칭 정의
using Pos = Vector;
