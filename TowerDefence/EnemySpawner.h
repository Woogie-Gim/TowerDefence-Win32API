#pragma once

class EnemySpawner
{
public:
    void SetInterval(float sec) { _interval = sec; }
    void SetSpawnX(float x) { _spawnX = x; }
    void SetSpawnY(float yMin, float yMax) { _spawnYMin = yMin; _spawnYMax = yMax; }

    void Update(float dt);

    // ����: ù �����ӿ� �ٷ� 1���� �̰� ������
    void Kickstart() { _acc = _interval; }

private:
    void SpawnOne();

private:
    float _acc = 0.f;
    float _interval = 0.8f;
    float _spawnX = -50.f;
    float _spawnYMin = 150.f;
    float _spawnYMax = 450.f;
};