#pragma once
#include "Object.h"

// GDI+ ���漱��
namespace Gdiplus { class Image; class ImageAttributes; }

class Monster : public Object
{
public:
	Monster();
	virtual ~Monster() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	// �̻����� �¾��� �� ȣ��
	void OnHit();

private:
	void LoadSprites();

    // ����
    enum class State { Walk, Hurt, FadeOut };
    State _state = State::Walk;

    // �ִϸ��̼�
    Gdiplus::Image* _walk1 = nullptr;
    Gdiplus::Image* _walk2 = nullptr;
    Gdiplus::Image* _hurt = nullptr;

    float _animAcc = 0.f;          // ������ ��ȯ �����ð�
    float _animInterval = 0.15f;   // �ȱ� ������ ��ȯ ����
    bool  _useFirst = true;        // walk1/walk2 ���

    // Hurt ���� �ð� �� ���̵�
    float _hurtTime = 0.18f;       // hurt ���� ǥ�ýð�
    float _hurtAcc = 0.f;

    float _alpha = 1.0f;           // 1.0 -> 0.0 ���� ���̵�
    float _fadeSpeed = 2.0f;       // �ʴ� ���� ���ҷ�

    // ���� ������(�ȼ�)
    int _renderW = 35;
    int _renderH = 48;

	Vector _lookPos = {};
	Vector _lookDir = {};
};

