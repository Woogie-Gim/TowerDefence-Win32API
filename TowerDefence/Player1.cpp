#include "pch.h"
#include "Player1.h"
#include "GdiPlusSafe.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Missile.h"
#include "Utils.h"
#include "Monster.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// 실행 파일(.exe) 기준 경로 유틸
extern std::wstring MakeAssetPath(const std::wstring& rel);

static inline float Deg2Rad(float d) { return d * 3.1415926535f / 180.0f; }

void Player1::Init()
{
    _pos = { 500.f, 500.f };
    _fireInterval = 7.0f;
    _fireAcc = 0.0f;
    _scale = 1.0f;

    _spreadDeg = 12.0f; // ★ 각도 스프레드 확실히 지정 (10~18도 취향껏)

    LoadSprite();
}

void Player1::Update()
{
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();
    _fireAcc += dt;

    // 타겟 탐색(가장 먼저 보이는 몬스터 1마리)
    const auto objects = GET_SINGLE(ObjectManager)->GetObjects();
    Object* target = nullptr;
    for (Object* o : objects)
    {
        if (!o) continue;
        if (o->GetObjectType() == GameObjectType::Monster) { target = o; break; }
    }

    // 조준 각도 갱신(타겟 없으면 기존 각도 유지)
    if (target)
    {
        const Pos tp = target->GetPos();
        float dx = tp.x - _pos.x;
        float dy = _pos.y - tp.y; // GDI 좌표 보정
        _barrelAngle = std::atan2(dy, dx);
    }

    auto& om = *GET_SINGLE(ObjectManager);

    // 버스트가 진행 중이면, 간격 타이머로 한 발씩 쏜다
    if (_burstRemain > 0)
    {
        _burstAcc += dt;
        if (_burstAcc >= _burstGap)
        {
            _burstAcc -= _burstGap;

            // 이번 발의 인덱스(0,1,2)로 각도 선택: 중앙, +스프레드, -스프레드
            int shotIdx = _burstCount - _burstRemain; // 0부터 시작
            float ang = _latchedAngle;
            if (shotIdx == 1) ang = _latchedAngle + Deg2Rad(_spreadDeg);
            else if (shotIdx == 2) ang = _latchedAngle - Deg2Rad(_spreadDeg);

            const Pos fire = GetFirePos(); // 총구 위치

            Missile* m = om.CreateObject<Missile>();
            m->SetPos(fire);
            m->SetAngle(ang);
            m->SetSpritePath(L"Assets\\Missile\\elephant.png");
            om.Add(m);

            _burstRemain--;
        }
        return; // 버스트 중에는 여기서 종료(추가 로직 없음)
    }

    //  버스트가 진행 중이 아니고, 발사 쿨이 돌았다면 새 버스트 시작
    if (target && _fireAcc >= _fireInterval)
    {
        _fireAcc -= _fireInterval;

        _latchedAngle = _barrelAngle; // 시작 시점의 각도 고정
        _burstRemain = _burstCount;  // 3발 쏠 준비
        _burstAcc = 0.f;          // 간격 타이머 리셋
    }
}

void Player1::Render(HDC hdc)
{
    if (_sprite)
    {
        Graphics g(hdc);
        g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        g.SetPixelOffsetMode(PixelOffsetModeHalf);

        const int targetW = static_cast<int>(46.5 * _scale);
        const int targetH = static_cast<int>(32 * _scale);
        const int left = static_cast<int>(_pos.x) - targetW / 2;
        const int top = static_cast<int>(_pos.y) - targetH / 2;
        Rect dest(left, top, targetW, targetH);
        g.DrawImage(_sprite, dest);
    }
    else
    {
        const int hw = 20, hh = 20;
        Rectangle(hdc, (int)(_pos.x - hw), (int)(_pos.y - hh), (int)(_pos.x + hw), (int)(_pos.y + hh));
    }
}

void Player1::LoadSprite()
{
    if (_sprite) return;
    const std::wstring full = MakeAssetPath(L"Assets\\Player\\elephant.png");
    if (!std::filesystem::exists(full)) return;

    auto* img = Gdiplus::Image::FromFile(full.c_str());
    if (img && img->GetLastStatus() == Ok) _sprite = img; else delete img;
}

Pos Player1::GetFirePos() const
{
    Pos fire = _pos;
    fire.x += _barrelLength * ::cos(_barrelAngle);
    fire.y -= _barrelLength * ::sin(_barrelAngle);
    return fire;
}
