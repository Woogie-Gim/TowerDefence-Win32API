#pragma once
#include "Object.h"

// GDI+ 전방선언 (헤더 오염 방지)
namespace Gdiplus { class Image; }

class Player : public Object
{
public:
	Player();
	virtual ~Player() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void LoadSprite();

	Pos GetFirePos();

public:
	float _barrelAngle = 0.f;
	float _barrelLength = 100.f;

private:
	float _fireInterval = 0.5f; // 발사 간격
	float _fireAcc = 0.f;		// 누적 시간
	Gdiplus::Image* _sprite = nullptr;
	float _scale = 1.0f;
};

