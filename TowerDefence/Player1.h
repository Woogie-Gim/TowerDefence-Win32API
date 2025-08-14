#pragma once
#include "Object.h"
#include <string>
#include <filesystem>
#include <windows.h>

namespace Gdiplus { class Image; }

class Player1 : public Object
{
public:
    Player1() : Object(GameObjectType::Player) {}
    ~Player1() override {}

    void Init() override;
    void Update() override;
    void Render(HDC hdc) override;

    void SetPos(const Pos& p) { _pos = p; }

private:
    void LoadSprite();
    Pos  GetFirePos() const;

private:
    // ���־�
    Gdiplus::Image* _sprite = nullptr;
    float _scale = 1.0f;

    // �߻�
    float _fireInterval = 1.0f;  // 1�ʸ���
    float _fireAcc = 0.0f;
    float _barrelAngle = 0.0f;  // ����
    float _barrelLength = 24.0f; // �߻籸 ��ġ ����

    // ����Ʈ(����) ����
    int   _burstCount = 3;     // �� ���� �� ��
    float _burstGap = 0.06f; // �� ���� ����(��) - ���ٴ� ����
    int   _burstRemain = 0;     // ���� �߼�
    float _burstAcc = 0.f;   // ���� Ÿ�̸�
    float _latchedAngle = 0.f;   // ����Ʈ ���� �� ������ ����(��鸲 ����)

    // Ʈ���ü� ���� ������(��)
    float _spreadDeg = 10.0f;
};
