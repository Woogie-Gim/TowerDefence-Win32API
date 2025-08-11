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
    // 몬스터 기본 스탯 설정
    _stat.hp = 100;
    _stat.maxHp = 100;
    _stat.speed = 100; // 픽셀/초 단위 이동 속도

    // 초기 위치 (왼쪽 화면 밖에서 시작)
    _pos = Pos{ -50, 300 };
}

void Monster::Update()
{
    // deltaTime 가져오기
    float dt = GET_SINGLE(TimeManager)->GetDeltaTime();

    // X 좌표를 속도 * 시간 만큼 증가
    _pos.x += _stat.speed * dt;

    // 화면 오른쪽 바깥으로 나가면 제거
    if (_pos.x > 850) // 화면 너비보다 여유있게
    {
        GET_SINGLE(ObjectManager)->Remove(this);
        return;
    }
}

void Monster::Render(HDC hdc)
{
    // 몬스터 원 그리기 (반지름 25)
    Utils::DrawCircle(hdc, _pos, 25);
}
