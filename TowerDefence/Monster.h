#pragma once
#include "Object.h"
#include <vector>
#include <functional>
#include <windows.h>

// GDI+ ���漱��
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

    // �ܺ� ����
    void OnHit();
    void SetPos(POINTF p);                            // �� ����� ���� �߰�
    void SetPathPixels(const std::vector<POINT>& pts);// �ȼ� ��������Ʈ ����
    void SetOnEscaped(std::function<void()> cb) { _onEscaped = std::move(cb); }

private:
    void LoadSprites();

    // ����
    enum class State { Walk, Hurt, FadeOut };
    State _state = State::Walk;

    // �ִϸ��̼�
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

    // ���� ������
    int _renderW = 35;
    int _renderH = 48;

    // ���/�̵� (�ϳ��� ����)
    std::vector<POINT> _waypoints;   // �ȼ� ��������Ʈ
    size_t _wpIndex = 0;             // ���� ��ǥ ��������Ʈ �ε���
    float  _reachRadius = 6.f;       // ���� ���� �ݰ�

    // Ż�� �ݹ�/���� �÷���
    std::function<void()> _onEscaped;
    bool _pendingDestroy = false;
};
