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

    // ���� ���(Grid ��ǥ) ��������
    const auto& path = _map->GetPath();
    if (path.empty()) return;

    // Grid �� Pixel ��������Ʈ�� ��ȯ
    std::vector<POINT> pixelPath;
    pixelPath.reserve(path.size());
    for (auto gp : path)
        pixelPath.push_back(Map::GridToPixelCenter(gp.x, gp.y));

    // ���� ��ġ = ù ��������Ʈ(��� ������)
    POINT p0 = pixelPath.front();

    // ���� ���� & ��� ����
    Monster* m = GET_SINGLE(ObjectManager)->CreateObject<Monster>();
    m->SetPos(POINTF{ (float)p0.x, (float)p0.y });
    m->SetPathPixels(pixelPath);

    if (onEnemyEscaped)
        m->SetOnEscaped(onEnemyEscaped);

    GET_SINGLE(ObjectManager)->Add(m);
}
