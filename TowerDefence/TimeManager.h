#pragma once

class TimeManager
{
	DECLARE_SINGLE(TimeManager); // �̱��� ��ũ��. �������� �ϳ��� �ν��Ͻ��� ������

public:
	void Init();	// ������ Ÿ�̸� �ʱ�ȭ
	void Update();	// �� �����Ӹ��� �ð� ���� �� FPS ���

	// ���� �����Ӱ� ���� ������ ������ �ð� ���� ��ȯ (�� ����)
	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	// �ʴ� ������ Ÿ�̸� ƽ ��
	uint64 _frequency = 0;
	// ���� ������ ������ ī���� ��
	uint64 _prevCount = 0;
	// ��� �ð� (��)
	float _deltaTime = 0.f;

private:
	// FPS ���� ������ �� ī��Ʈ
	uint32 _frameCount = 0;
	// ������ �ð� (��)
	float _frameTime = 0.f;
	// 1�� ���� FPS ��
	uint32 _fps = 0;
};