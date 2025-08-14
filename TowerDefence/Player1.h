#pragma once
#include "Object.h"
#include <string>
#include <filesystem>
#include <windows.h>

namespace Gdiplus { class Image; }

class Player1 : public Object
{
public:
    Player1() : Object(GameObjectType::Player) {}
    ~Player1() override {}

    void Init() override;
    void Update() override;
    void Render(HDC hdc) override;

    void SetPos(const Pos& p) { _pos = p; }

private:
    void LoadSprite();
    Pos  GetFirePos() const;

private:
    // 비주얼
    Gdiplus::Image* _sprite = nullptr;
    float _scale = 1.0f;

    // 발사
    float _fireInterval = 1.0f;  // 1초마다
    float _fireAcc = 0.0f;
    float _barrelAngle = 0.0f;  // 라디안
    float _barrelLength = 24.0f; // 발사구 위치 보정

    // 버스트(연사) 설정
    int   _burstCount = 3;     // 한 번에 몇 발
    float _burstGap = 0.06f; // 발 사이 간격(초) - 따다닥 템포
    int   _burstRemain = 0;     // 남은 발수
    float _burstAcc = 0.f;   // 간격 타이머
    float _latchedAngle = 0.f;   // 버스트 시작 시 고정한 각도(흔들림 방지)

    // 트리플샷 각도 오프셋(도)
    float _spreadDeg = 10.0f;
};
