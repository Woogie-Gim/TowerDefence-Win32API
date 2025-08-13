#pragma once

class Map;
class Monster;

class EnemySpawner
{
public:
    void SetInterval(float sec) { _interval = sec; }

    // 맵을 넘겨주면 스포너가 맵의 Path를 사용함
    void SetMap(Map* m) { _map = m; }

    void Update(float dt);

    // 첫 프레임에 바로 1마리 뽑고 싶으면 사용
    void Kickstart() { _acc = _interval; }

private:
    void SpawnOne();

private:
    float _acc = 0.f;
    float _interval = 0.8f;

    Map* _map = nullptr; // 경로를 알아내기 위한 맵 참조
};