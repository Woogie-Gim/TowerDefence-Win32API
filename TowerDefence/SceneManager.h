#pragma once

// Scene�� �����ϴ� �Ŵ��� Ŭ����
class SceneManager
{
	DECLARE_SINGLE(SceneManager); // �̱��� ��ũ��

public:
	// Scene �Ŵ��� �ʱ�ȭ
	void Init();
	// ���� Scene ������Ʈ ȣ��
	void Update();
	// ���� Scene ������ ȣ��
	void Render(HDC hdc);
	// ���� Scene ����
	void Clear();

public:
	// Scene ����
	void ChangeScene(SceneType sceneType);

private:
	class Scene* _scene;             // ���� Ȱ��ȭ�� Scene
	SceneType _sceneType = SceneType::None;  // ���� Scene Ÿ��
};
