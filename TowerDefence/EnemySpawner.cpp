#include "pch.h"
#include "EnemySpawner.h"
#include "ObjectManager.h"
#include "Monster.h"

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
    float t = (std::rand() % 10000) / 10000.0f;
    float spawnY = _spawnYMin + (_spawnYMax - _spawnYMin) * t;

    Monster* m = GET_SINGLE(ObjectManager)->CreateObject<Monster>();
    m->SetPos({ _spawnX, spawnY });
    GET_SINGLE(ObjectManager)->Add(m);
}
