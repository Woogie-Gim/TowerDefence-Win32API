#pragma once
#include "Object.h"

// GDI+ 전방선언
namespace Gdiplus { class Image; class ImageAttributes; }

class Monster : public Object
{
public:
	Monster();
	virtual ~Monster() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	// 미사일이 맞았을 때 호출
	void OnHit();

    // 스포너가 픽셀 좌표의 웨이포인트를 세팅해줌
    void SetPathPixels(const std::vector<POINT>& pts);

private:
	void LoadSprites();

    // 상태
    enum class State { Walk, Hurt, FadeOut };
    State _state = State::Walk;

    // 애니메이션
    Gdiplus::Image* _walk1 = nullptr;
    Gdiplus::Image* _walk2 = nullptr;
    Gdiplus::Image* _hurt = nullptr;

    float _animAcc = 0.f;          // 프레임 전환 누적시간
    float _animInterval = 0.15f;   // 걷기 프레임 전환 간격
    bool  _useFirst = true;        // walk1/walk2 토글

    // Hurt 유지 시간 및 페이드
    float _hurtTime = 0.18f;       // hurt 고정 표시시간
    float _hurtAcc = 0.f;

    float _alpha = 1.0f;           // 1.0 -> 0.0 으로 페이드
    float _fadeSpeed = 2.0f;       // 초당 알파 감소량

    // 렌더 사이즈(픽셀)
    int _renderW = 35;
    int _renderH = 48;

    // 웨이 포인트 경로
    std::vector<POINT> _waypoints;  // 픽셀 좌표
    size_t _wpIndex = 0;            // 현재 목표 웨이포인트 인덱스
    float  _reachRadius = 6.f;      // 이 거리 이내면 다음 점으로
};

