#include "pch.h"
#include "EnemySpawner.h"
#include "Map.h"
#include "Monster.h"
#include "ObjectManager.h"

void EnemySpawner::Update(float dt)
{
    _acc += dt;
    if (_acc >= _interval)
    {
        _acc -= _interval;
        SpawnOne();
    }
}

void EnemySpawner::SpawnOne()
{
    if (_map == nullptr) return;

    // 맵의 경로(Grid 좌표) 가져오기
    const auto& path = _map->GetPath();
    if (path.empty()) return;

    // Grid → Pixel 웨이포인트로 변환
    std::vector<POINT> pixelPath;
    pixelPath.reserve(path.size());
    for (auto gp : path)
        pixelPath.push_back(Map::GridToPixelCenter(gp.x, gp.y));

    // 스폰 위치 = 첫 웨이포인트(경로 시작점)
    POINT p0 = pixelPath.front();

    // 몬스터 생성 & 경로 세팅
    Monster* m = GET_SINGLE(ObjectManager)->CreateObject<Monster>();
    m->SetPos(POINTF{ (float)p0.x, (float)p0.y });
    m->SetPathPixels(pixelPath);

    if (onEnemyEscaped)
        m->SetOnEscaped(onEnemyEscaped);

    GET_SINGLE(ObjectManager)->Add(m);
}
