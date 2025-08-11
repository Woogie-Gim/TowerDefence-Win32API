#include "pch.h"
#include "Utils.h"
#include "Monster.h"
#include "TimeManager.h"
#include "ObjectManager.h"

Monster::Monster() : Object(GameObjectType::Monster)
{

}

Monster::~Monster()
{

}

void Monster::Init()
{
    // ���� �⺻ ���� ����
    _stat.hp = 100;
    _stat.maxHp = 100;
    _stat.speed = 100; // �ȼ�/�� ���� �̵� �ӵ�

    // �ʱ� ��ġ (���� ȭ�� �ۿ��� ����)
    _pos = Pos{ -50, 300 };
}

void Monster::Update()
{
    // deltaTime ��������
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();

    // X ��ǥ�� �ӵ� * �ð� ��ŭ ����
    _pos.x += _stat.speed * dt;

    // ȭ�� ������ �ٱ����� ������ ����
    if (_pos.x > 850) // ȭ�� �ʺ񺸴� �����ְ�
    {
        GET_SINGLE(ObjectManager)->Remove(this);
        return;
    }
}

void Monster::Render(HDC hdc)
{
    // ���� �� �׸��� (������ 25)
    Utils::DrawCircle(hdc, _pos, 25);
}
