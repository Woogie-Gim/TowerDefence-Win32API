#include "pch.h"
#include "Monster.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "GdiPlusSafe.h" 
#include <filesystem>

extern std::wstring MakeAssetPath(const std::wstring& rel); 

static std::wstring P(const wchar_t* file)
{
    return MakeAssetPath(std::wstring(L"Assets\\Monster\\") + file);
}

Monster::Monster() : Object(GameObjectType::Monster) 
{

}

Monster::~Monster()
{

}

void Monster::Init()
{
    _stat.hp = 100;
    _stat.maxHp = 100;
    _stat.speed = 100.f;

    _pos = Pos{ -50.f, 300.f };

    LoadSprites();
    _state = State::Walk;
    _alpha = 1.0f;
    _hurtAcc = 0.f;
    _animAcc = 0.f;
}

void Monster::Update()
{
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();

    switch (_state)
    {
    case State::Walk:
        // �̵�
        _pos.x += _stat.speed * dt;

        // �ȱ� �ִϸ��̼� ���
        _animAcc += dt;
        if (_animAcc >= _animInterval)
        {
            _animAcc -= _animInterval;
            _useFirst = !_useFirst;
        }

        // ȭ�� ������ ������ ����
        if (_pos.x > 850.f)
        {
            GET_SINGLE(ObjectManager)->Remove(this);
            return;
        }
        break;

    case State::Hurt:
        // ���� �ð� Hurt �̹��� ����
        _hurtAcc += dt;
        if (_hurtAcc >= _hurtTime)
        {
            _state = State::FadeOut;
            _hurtAcc = 0.f;
        }
        break;

    case State::FadeOut:
        // ������ ����ȭ
        _alpha -= _fadeSpeed * dt;
        if (_alpha <= 0.f)
        {
            GET_SINGLE(ObjectManager)->Remove(this);
            return;
        }
        break;
    }
}

void Monster::LoadSprites()
{
    if (_walk1 && _walk2 && _hurt) return;

    auto load = [](const std::wstring& path) -> Gdiplus::Image*
    {
        if (!std::filesystem::exists(path)) return nullptr;
        auto* img = Gdiplus::Image::FromFile(path.c_str());
        if (img && img->GetLastStatus() == Gdiplus::Ok) return img;
        delete img;
        return nullptr;
    };

    _walk1 = load(P(L"alienPink_walk1.png"));
    _walk2 = load(P(L"alienPink_walk2.png"));
    _hurt = load(P(L"alienPink_hurt.png"));
}

void Monster::Render(HDC hdc) 
{
    Gdiplus::Image* img = nullptr;

    if (_state == State::Hurt || _state == State::FadeOut)
    {
        img = _hurt ? _hurt : _walk1;
    }
    else 
    {
        img = (_useFirst ? _walk1 : _walk2);
        if (!img) img = _walk1 ? _walk1 : _walk2;
    }

    if (!img) 
    {
        HBRUSH b = (HBRUSH)GetStockObject(GRAY_BRUSH);
        HBRUSH old = (HBRUSH)SelectObject(hdc, b);
        Ellipse(hdc, (int)(_pos.x - 25), (int)(_pos.y - 25), (int)(_pos.x + 25), (int)(_pos.y + 25));
        SelectObject(hdc, old);
        return;
    }

    Gdiplus::Graphics g(hdc);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);

    // ���� ����
    Gdiplus::ImageAttributes attrs;
    Gdiplus::ColorMatrix cm =
    {
        1,0,0,0,0,
        0,1,0,0,0,
        0,0,1,0,0,
        0,0,0,_alpha,0,
        0,0,0,0,1
    };
    attrs.SetColorMatrix(&cm, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

    int left = (int)_pos.x - _renderW / 2;
    int top = (int)_pos.y - _renderH / 2;
    Gdiplus::Rect dest(left, top, _renderW, _renderH);

    g.DrawImage(img, dest, 0, 0, img->GetWidth(), img->GetHeight(), Gdiplus::UnitPixel, &attrs);
}

void Monster::OnHit()
{
    // �̹� ������� ���̸� �ߺ� ó�� ����
    if (_state == State::FadeOut)
        return;

    // �ϴ� Hurt �������� ��� �����ְ�
    _state = State::Hurt;
    _hurtAcc = 0.f;   // Hurt ���� Ÿ�̸� ����
    _alpha = 1.0f;  // ���� �ʱ�ȭ
}