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

// ���� ����(.exe) ���� ��� ��ƿ
extern std::wstring MakeAssetPath(const std::wstring& rel);

static inline float Deg2Rad(float d) { return d * 3.1415926535f / 180.0f; }

void Player1::Init()
{
    _pos = { 500.f, 500.f };
    _fireInterval = 7.0f;
    _fireAcc = 0.0f;
    _scale = 1.0f;

    _spreadDeg = 12.0f; // �� ���� �������� Ȯ���� ���� (10~18�� ���ⲯ)

    LoadSprite();
}

void Player1::Update()
{
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();
    _fireAcc += dt;

    // Ÿ�� Ž��(���� ���� ���̴� ���� 1����)
    const auto objects = GET_SINGLE(ObjectManager)->GetObjects();
    Object* target = nullptr;
    for (Object* o : objects)
    {
        if (!o) continue;
        if (o->GetObjectType() == GameObjectType::Monster) { target = o; break; }
    }

    // ���� ���� ����(Ÿ�� ������ ���� ���� ����)
    if (target)
    {
        const Pos tp = target->GetPos();
        float dx = tp.x - _pos.x;
        float dy = _pos.y - tp.y; // GDI ��ǥ ����
        _barrelAngle = std::atan2(dy, dx);
    }

    auto& om = *GET_SINGLE(ObjectManager);

    // ����Ʈ�� ���� ���̸�, ���� Ÿ�̸ӷ� �� �߾� ���
    if (_burstRemain > 0)
    {
        _burstAcc += dt;
        if (_burstAcc >= _burstGap)
        {
            _burstAcc -= _burstGap;

            // �̹� ���� �ε���(0,1,2)�� ���� ����: �߾�, +��������, -��������
            int shotIdx = _burstCount - _burstRemain; // 0���� ����
            float ang = _latchedAngle;
            if (shotIdx == 1) ang = _latchedAngle + Deg2Rad(_spreadDeg);
            else if (shotIdx == 2) ang = _latchedAngle - Deg2Rad(_spreadDeg);

            const Pos fire = GetFirePos(); // �ѱ� ��ġ

            Missile* m = om.CreateObject<Missile>();
            m->SetPos(fire);
            m->SetAngle(ang);
            m->SetSpritePath(L"Assets\\Missile\\elephant.png");
            om.Add(m);

            _burstRemain--;
        }
        return; // ����Ʈ �߿��� ���⼭ ����(�߰� ���� ����)
    }

    //  ����Ʈ�� ���� ���� �ƴϰ�, �߻� ���� ���Ҵٸ� �� ����Ʈ ����
    if (target && _fireAcc >= _fireInterval)
    {
        _fireAcc -= _fireInterval;

        _latchedAngle = _barrelAngle; // ���� ������ ���� ����
        _burstRemain = _burstCount;  // 3�� �� �غ�
        _burstAcc = 0.f;          // ���� Ÿ�̸� ����
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
