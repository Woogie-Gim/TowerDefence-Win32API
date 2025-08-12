#include "pch.h"
#include "Missile.h"
#include "GdiPlusSafe.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Utils.h"

using namespace Gdiplus;

// TowerDefence.cpp에 정의된 경로 유틸 사용
extern std::wstring MakeAssetPath(const std::wstring& rel);

Missile::Missile() : Object(GameObjectType::Projectile) {}

Missile::~Missile() {}

void Missile::Init()
{
    // 기본 스탯 설정
    _stat.hp = 1;
    _stat.maxHp = 1;
    _stat.speed = 600.f; // 이동 속도

    _sumTime = 0.f;      // 발사 후 경과 시간
    _target = nullptr;   // 현재 추적 중인 목표

    LoadSprite();
}

void Missile::Update()
{
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();
    _sumTime += dt;

    // 충돌 판정 반경 및 수명 제한
    constexpr float kHitRadius = 12.f;
    constexpr float kHitRadius2 = kHitRadius * kHitRadius; // 제곱 반경(√ 연산 회피)
    constexpr float kMaxLifeTime = 3.0f;                    // 최대 생존 시간(초)
    constexpr float kSpawnGrace = 0.05f;  // 스폰 후 50ms 충돌 무시

    // 현재 게임 내 모든 오브젝트 참조
    const auto objects = GET_SINGLE(ObjectManager)->GetObjects();

    // 타겟 유효성 검사 (삭제되었으면 nullptr로 초기화)
    if (_target)
    {
        auto it = std::find(objects.begin(), objects.end(), _target);
        if (it == objects.end())
            _target = nullptr;
    }

    Object* nearest = nullptr;     // 가장 가까운 몬스터
    float   nearestD2 = FLT_MAX;     // 그 거리의 제곱값
    Object* hitObject = nullptr;     // 충돌 대상

    Vector p1 = GetPos(); // 미사일 위치

    // 모든 오브젝트 검사
    for (Object* obj : objects)
    {
        if (obj == this) continue;                               // 자기 자신 제외
        if (obj->GetObjectType() != GameObjectType::Monster) continue; // 몬스터만 검사

        Vector p2 = obj->GetPos();
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float d2 = dx * dx + dy * dy; // 거리의 제곱

        // 충돌 판정
        if (_sumTime >= kSpawnGrace && d2 <= kHitRadius2)
        {
            hitObject = obj;
            break;
        }

        // 가장 가까운 몬스터 찾기
        if (d2 < nearestD2)
        {
            nearestD2 = d2;
            nearest = obj;
        }
    }

    // 충돌 발생 시 자신과 상대 삭제
    if (hitObject)
    {
        if (hitObject->GetObjectType() == GameObjectType::Monster)
            static_cast<Monster*>(hitObject)->OnHit(); // Hurt -> FadeOut

        GET_SINGLE(ObjectManager)->Remove(this); // 미사일만 제거
        return;
    }

    // 타겟이 없으면 가장 가까운 몬스터를 타겟으로 지정
    if (_target == nullptr && nearest != nullptr)
        _target = nearest;

    // 타겟이 없으면 직진
    if (_target == nullptr)
    {
        _pos.x += _stat.speed * dt * cosf(_angle);
        _pos.y -= _stat.speed * dt * sinf(_angle);
    }
    // 타겟이 있으면 해당 방향으로 이동
    else
    {
        Vector tp = _target->GetPos();
        float dx = tp.x - _pos.x;
        float dy = tp.y - _pos.y;
        float len = sqrtf(dx * dx + dy * dy);
        if (len > 0.0001f) // 0으로 나누기 방지
        {
            float nx = dx / len; // 단위벡터 x
            float ny = dy / len; // 단위벡터 y
            _pos.x += nx * _stat.speed * dt;
            _pos.y += ny * _stat.speed * dt;
        }
    }

    // 수명 초과 또는 화면 밖으로 벗어나면 삭제
    if (_sumTime >= kMaxLifeTime || _pos.y < -200)
    {
        GET_SINGLE(ObjectManager)->Remove(this);
        return;
    }
}

void Missile::Render(HDC hdc)
{
    if (_sprite)
    {
        Graphics g(hdc);
        g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        g.SetPixelOffsetMode(PixelOffsetModeHalf);

        const int targetW = static_cast<int>(20 * _scale);
        const int targetH = static_cast<int>(16 * _scale);

        const int left = static_cast<int>(_pos.x) - targetW / 2;
        const int top = static_cast<int>(_pos.y) - targetH / 2;

        Rect dest(left, top, targetW, targetH);
        g.DrawImage(_sprite, dest);
    }
    else
    {
        // 스프라이트 로드 실패 시 원형 표시
        Utils::DrawCircle(hdc, _pos, 12);
    }

}

void Missile::LoadSprite()
{
    if (_sprite) return;

    // 실행 파일(.exe) 폴더 기준 상대 경로
    const std::wstring full = MakeAssetPath(L"Assets\\Missile\\cow.png");

    // 파일이 없으면 바로 종료 (렌더에서 네모로 대체됨)
    if (!std::filesystem::exists(full))
        return;

    // GDI+로 이미지 로드 (알파 자동 처리)
    Gdiplus::Image* img = Gdiplus::Image::FromFile(full.c_str());
    if (!img) return;

    if (img->GetLastStatus() == Ok)
        _sprite = img;
    else
        delete img; // 실패 시 정리
}
