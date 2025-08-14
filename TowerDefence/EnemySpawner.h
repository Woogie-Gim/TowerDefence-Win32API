#pragma once
#include <functional>

class Map;
class Monster;

class EnemySpawner
{
public:
    void SetInterval(float sec) { _interval = sec; }

    // ���� �Ѱ��ָ� �����ʰ� ���� Path�� �����
    void SetMap(Map* m) { _map = m; }

    void Update(float dt);

    // ù �����ӿ� �ٷ� 1���� �̰� ������ ���
    void Kickstart() { _acc = _interval; }

    void SetOnEnemyEscaped(std::function<void()> cb) { onEnemyEscaped = std::move(cb); }

    void Reset() { _acc = 0.f; }

private:
    void SpawnOne();

private:
    float _acc = 0.f;
    float _interval = 0.8f;

    Map* _map = nullptr; // ��θ� �˾Ƴ��� ���� �� ����

    std::function<void()> onEnemyEscaped;
};