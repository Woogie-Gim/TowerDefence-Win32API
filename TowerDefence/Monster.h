#pragma once
#include "Object.h"
#include <vector>
#include <functional>
#include <windows.h>

// GDI+ 전방선언
namespace Gdiplus { class Image; class ImageAttributes; }

struct POINTF
{
    float x;
    float y;
};

class Monster : public Object
{
public:
    Monster();
    virtual ~Monster() override;

    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    // 외부 제어
    void OnHit();
    void SetPos(POINTF p);                            // ★ 헤더에 선언 추가
    void SetPathPixels(const std::vector<POINT>& pts);// 픽셀 웨이포인트 세팅
    void SetOnEscaped(std::function<void()> cb) { _onEscaped = std::move(cb); }

private:
    void LoadSprites();

    // 상태
    enum class State { Walk, Hurt, FadeOut };
    State _state = State::Walk;

    // 애니메이션
    Gdiplus::Image* _walk1 = nullptr;
    Gdiplus::Image* _walk2 = nullptr;
    Gdiplus::Image* _hurt = nullptr;

    float _animAcc = 0.f;
    float _animInterval = 0.15f;
    bool  _useFirst = true;

    // Hurt/Fade
    float _hurtTime = 0.18f;
    float _hurtAcc = 0.f;
    float _alpha = 1.0f;
    float _fadeSpeed = 2.0f;

    // 렌더 사이즈
    int _renderW = 35;
    int _renderH = 48;

    // 경로/이동 (하나로 정리)
    std::vector<POINT> _waypoints;   // 픽셀 웨이포인트
    size_t _wpIndex = 0;             // 현재 목표 웨이포인트 인덱스
    float  _reachRadius = 6.f;       // 도착 판정 반경

    // 탈출 콜백/제거 플래그
    std::function<void()> _onEscaped;
    bool _pendingDestroy = false;
};
