#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	// ������ Ÿ�̸��� �ʴ� ī��Ʈ ���� ������
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	// ���� �ð� ī���� ���� �����Ͽ� ���� �������� ����
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));
}

void TimeManager::Update()
{
	uint64 currentCount;

	// ���� �ð� ī���� �� ��������
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// ���� �����Ӱ� ���� ������ ������ ��� �ð� ���
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	// FPS ����� ���� ���� ������ ���� �ð� ����
	_frameCount++;
	_frameTime += _deltaTime;

	// ���� �ð��� 1�� �̻��̸� FPS ��� �� �ʱ�ȭ
	if (_frameTime >= 1.f)
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}
