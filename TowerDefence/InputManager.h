#pragma once

// ����ϰ��� �ϴ� Ű�� ������ ����
enum class KeyType
{
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Rihgt = VK_RIGHT,
	SpaceBar = VK_SPACE,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	Q = 'Q',
	E = 'E',
};

// Ű�� ���¸� ��Ÿ���� ������
enum class KeyState
{
	None,   // �ƹ� ���� ����
	Press,  // ������ ����
	Down,   // �̹� �����ӿ��� ó�� ����
	Up,     // �̹� �����ӿ��� ó�� ����

	End     // ���� ���� (�ݺ��� ��� ���)
};

// �迭 ũ�� ���ǿ� ���
enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX) + 1,           // Ű �ڵ� �ִ� �� (0~255)
	KEY_STATE_COUNT = static_cast<int32>(KeyState::End)           // Ű ���� ���� ��
};

class InputManager
{
	DECLARE_SINGLE(InputManager); // �̱��� ��ũ��

public:
	// �ʱ�ȭ: ������ �ڵ��� ����
	void Init(HWND hwnd);
	// �� ������ ȣ���Ͽ� Ű ���� ����
	void Update();
	// Ű�� ������ �ִ� ���ΰ�?
	bool GetButton(KeyType key) { return GetState(key) == KeyState::Press; }
	// Ű�� �̹� �����ӿ� �����°�?
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }
	// Ű�� �̹� �����ӿ� �����°�?
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }

	// ���콺 ���� ��ġ ��ȯ
	POINT GetMousePos() { return _mousePos; }

private:
	// Ư�� Ű�� ���� ���¸� ��ȯ
	KeyState GetState(KeyType key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd = 0;                  // �Է��� ���� ������ �ڵ�
	vector<KeyState> _states;       // ��ü Ű�� ���¸� �����ϴ� �迭 (256��)
	POINT _mousePos;                // ���콺 ��ġ
};
