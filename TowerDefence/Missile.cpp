#include "pch.h"
#include "Missile.h"
#include "GdiPlusSafe.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Utils.h"

using namespace Gdiplus;

// TowerDefence.cpp�� ���ǵ� ��� ��ƿ ���
extern std::wstring MakeAssetPath(const std::wstring& rel);

Missile::Missile() : Object(GameObjectType::Projectile) {}

Missile::~Missile() {}

void Missile::Init()
{
    // �⺻ ���� ����
    _stat.hp = 1;
    _stat.maxHp = 1;
    _stat.speed = 600.f; // �̵� �ӵ�

    _sumTime = 0.f;      // �߻� �� ��� �ð�
    _target = nullptr;   // ���� ���� ���� ��ǥ

    LoadSprite();
}

void Missile::Update()
{
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();
    _sumTime += dt;

    // �浹 ���� �ݰ� �� ���� ����
    constexpr float kHitRadius = 12.f;
    constexpr float kHitRadius2 = kHitRadius * kHitRadius; // ���� �ݰ�(�� ���� ȸ��)
    constexpr float kMaxLifeTime = 3.0f;                    // �ִ� ���� �ð�(��)
    constexpr float kSpawnGrace = 0.05f;  // ���� �� 50ms �浹 ����

    // ���� ���� �� ��� ������Ʈ ����
    const auto objects = GET_SINGLE(ObjectManager)->GetObjects();

    // Ÿ�� ��ȿ�� �˻� (�����Ǿ����� nullptr�� �ʱ�ȭ)
    if (_target)
    {
        auto it = std::find(objects.begin(), objects.end(), _target);
        if (it == objects.end())
            _target = nullptr;
    }

    Object* nearest = nullptr;     // ���� ����� ����
    float   nearestD2 = FLT_MAX;     // �� �Ÿ��� ������
    Object* hitObject = nullptr;     // �浹 ���

    Vector p1 = GetPos(); // �̻��� ��ġ

    // ��� ������Ʈ �˻�
    for (Object* obj : objects)
    {
        if (obj == this) continue;                               // �ڱ� �ڽ� ����
        if (obj->GetObjectType() != GameObjectType::Monster) continue; // ���͸� �˻�

        Vector p2 = obj->GetPos();
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float d2 = dx * dx + dy * dy; // �Ÿ��� ����

        // �浹 ����
        if (_sumTime >= kSpawnGrace && d2 <= kHitRadius2)
        {
            hitObject = obj;
            break;
        }

        // ���� ����� ���� ã��
        if (d2 < nearestD2)
        {
            nearestD2 = d2;
            nearest = obj;
        }
    }

    // �浹 �߻� �� �ڽŰ� ��� ����
    if (hitObject)
    {
        if (hitObject->GetObjectType() == GameObjectType::Monster)
            static_cast<Monster*>(hitObject)->OnHit(); // Hurt -> FadeOut

        GET_SINGLE(ObjectManager)->Remove(this); // �̻��ϸ� ����
        return;
    }

    // Ÿ���� ������ ���� ����� ���͸� Ÿ������ ����
    if (_target == nullptr && nearest != nullptr)
        _target = nearest;

    // Ÿ���� ������ ����
    if (_target == nullptr)
    {
        _pos.x += _stat.speed * dt * cosf(_angle);
        _pos.y -= _stat.speed * dt * sinf(_angle);
    }
    // Ÿ���� ������ �ش� �������� �̵�
    else
    {
        Vector tp = _target->GetPos();
        float dx = tp.x - _pos.x;
        float dy = tp.y - _pos.y;
        float len = sqrtf(dx * dx + dy * dy);
        if (len > 0.0001f) // 0���� ������ ����
        {
            float nx = dx / len; // �������� x
            float ny = dy / len; // �������� y
            _pos.x += nx * _stat.speed * dt;
            _pos.y += ny * _stat.speed * dt;
        }
    }

    // ���� �ʰ� �Ǵ� ȭ�� ������ ����� ����
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
        // ��������Ʈ �ε� ���� �� ���� ǥ��
        Utils::DrawCircle(hdc, _pos, 12);
    }

}

void Missile::LoadSprite()
{
    if (_sprite) return;

    // ���� ����(.exe) ���� ���� ��� ���
    const std::wstring full = MakeAssetPath(L"Assets\\Missile\\cow.png");

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
