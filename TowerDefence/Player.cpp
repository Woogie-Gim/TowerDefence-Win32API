#include "pch.h"
#include "Player.h"
#include "GdiPlusSafe.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Missile.h"
#include "ObjectManager.h"
#include "Utils.h"
#include <filesystem>
#include <string>

using namespace Gdiplus;

// TowerDefence.cpp�� ���ǵ� ��� ��ƿ ���
extern std::wstring MakeAssetPath(const std::wstring& rel);


Player::Player() : Object(GameObjectType::Player)
{

}

Player::~Player()
{

}

void Player::Init()
{
    _pos = { 400.f, 300.f };
    _stat.speed = 0.f;
    _fireInterval = 1.0f;
    _fireAcc = 0.f;

    // ��������Ʈ�� �� ���� �ε�
    LoadSprite();

    // �ʿ��ϸ� ǥ�� ���� ����
    _scale = 1.0f;
}

void Player::Update()
{
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();
    _fireAcc += dt;

    const auto objects = GET_SINGLE(ObjectManager)->GetObjects();
    Object* target = nullptr;

    for (Object* o : objects)
    {
        if (!o) continue;
        if (o->GetObjectType() == GameObjectType::Monster)
        {
            target = o;
            break;
        }
    }

    if (!target) return;

    const Pos tp = target->GetPos();
    float dx = tp.x - _pos.x;
    float dy = _pos.y - tp.y; // GDI: y �Ʒ��� ���� �� ���� + �� ���� ���� ��ȣ ����
    _barrelAngle = std::atan2(dy, dx);

    if (_fireAcc >= _fireInterval)
    {
        _fireAcc -= _fireInterval;

        Missile* m = GET_SINGLE(ObjectManager)->CreateObject<Missile>();
        m->SetPos(GetFirePos());
        m->SetAngle(_barrelAngle);
        GET_SINGLE(ObjectManager)->Add(m);
    }
}

void Player::Render(HDC hdc)
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
        // ��������Ʈ �ε� ���� �� ����ȭ�� �׸�
        const int halfW = 20, halfH = 20;
        RECT r{ (LONG)(_pos.x - halfW), (LONG)(_pos.y - halfH),
                (LONG)(_pos.x + halfW), (LONG)(_pos.y + halfH) };
        ::Rectangle(hdc, r.left, r.top, r.right, r.bottom);
    }

    // ���� ����
    const Pos fire = GetFirePos();
    Utils::DrawLine(hdc, _pos, fire);
}

void Player::LoadSprite()
{
    if (_sprite) return;

    // ���� ����(.exe) ���� ���� ��� ���
    const std::wstring full = MakeAssetPath(L"Assets\\Player\\cow.png");

    // ������ ������ �ٷ� ���� (�������� �׸�� ��ü��)
    if (!std::filesystem::exists(full))
        return;

    // GDI+�� �̹��� �ε� (���� �ڵ� ó��)
    Gdiplus::Image* img = Gdiplus::Image::FromFile(full.c_str());
    if (!img) return;

    if (img->GetLastStatus() == Ok)
        _sprite = img;
    else
        delete img; // ���� �� ����
}

Pos Player::GetFirePos()
{
    Pos firePos = _pos;
    firePos.x += _barrelLength * ::cos(_barrelAngle);
    firePos.y -= _barrelLength * ::sin(_barrelAngle);
    return firePos;
}
