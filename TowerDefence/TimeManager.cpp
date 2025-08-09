#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	// 고정밀 타이머의 초당 카운트 수를 가져옴
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	// 현재 시간 카운터 값을 저장하여 기준 시점으로 설정
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));
}

void TimeManager::Update()
{
	uint64 currentCount;

	// 현재 시간 카운터 값 가져오기
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// 이전 프레임과 현재 프레임 사이의 경과 시간 계산
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	// FPS 계산을 위한 누적 프레임 수와 시간 증가
	_frameCount++;
	_frameTime += _deltaTime;

	// 누적 시간이 1초 이상이면 FPS 계산 후 초기화
	if (_frameTime >= 1.f)
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}
