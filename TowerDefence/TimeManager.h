#pragma once

class TimeManager
{
	DECLARE_SINGLE(TimeManager); // 싱글톤 매크로. 전역에서 하나의 인스턴스를 유지함

public:
	void Init();	// 고정밀 타이머 초기화
	void Update();	// 매 프레임마다 시간 측정 및 FPS 계산

	// 지난 프레임과 현재 프레임 사이의 시간 차를 반환 (초 단위)
	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	// 초당 고정밀 타이머 틱 수
	uint64 _frequency = 0;
	// 이전 프레임 시점의 카운터 값
	uint64 _prevCount = 0;
	// 경과 시간 (초)
	float _deltaTime = 0.f;

private:
	// FPS 계산용 프레임 수 카운트
	uint32 _frameCount = 0;
	// 누적된 시간 (초)
	float _frameTime = 0.f;
	// 1초 기준 FPS 값
	uint32 _fps = 0;
};