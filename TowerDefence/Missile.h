#pragma once
#include "Object.h"

// GDI+ 전방선언 (헤더 오염 방지)
namespace Gdiplus { class Image; }

class Missile : public Object
{
public:
	Missile();
	virtual ~Missile() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void LoadSprite();

	void SetAngle(float angle) { _angle = angle; }
	void SetTarget(Object* target) { _target = target; }

protected:
	float _angle = 0.f;

	float _sumTime = 0.f;
	Object* _target = nullptr;
	Gdiplus::Image* _sprite = nullptr;
	float _scale = 1.0f;
};

