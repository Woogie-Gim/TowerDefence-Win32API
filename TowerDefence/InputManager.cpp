#include "pch.h"
#include "InputManager.h"

// �ʱ�ȭ: ������ �ڵ��� �����ϰ� Ű ���� �迭 �ʱ�ȭ
void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;

	// ��� Ű�� ���¸� None���� �ʱ�ȭ
	_states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::Update()
{
	// ��ü Ű������ ���� ���¸� ������ �迭 (256��)
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	// ���� Ű���� ���¸� �����´� (���� �� return)
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// 0~255 ��� Ű �˻�
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// �ش� Ű�� ���ȴ��� Ȯ�� (�ֻ��� ��Ʈ�� 1�̸� ���� ����)
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// ���� �����ӿ����� ������ �־��ٸ� Press
			if (state == KeyState::Press || state == KeyState::Down)
			{
				state = KeyState::Press;
			}
			// �̹� �����ӿ� ó�� ���� ��� Down
			else
			{
				state = KeyState::Down;
			}
		}
		else
		{
			KeyState& state = _states[key];

			// ���� �����ӿ��� ������ �־����� ������ �� ��� �� Up
			if (state == KeyState::Press || state == KeyState::Down)
			{
				state = KeyState::Up;
			}
			// �������� ������ �ʾҴ� ��� �� None
			else
			{
				state = KeyState::None;
			}
		}
	}

	// ���� ���콺 ��ġ�� Ŭ���̾�Ʈ ��ǥ��� ������
	::GetCursorPos(&_mousePos);               // ȭ�� ��ǥ��� ���콺 ��ġ Ȯ��
	::ScreenToClient(_hwnd, &_mousePos);      // Ŭ���̾�Ʈ ���� ��ǥ�� ��ȯ
}
