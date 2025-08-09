#include "pch.h"
#include "InputManager.h"

// 초기화: 윈도우 핸들을 저장하고 키 상태 배열 초기화
void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;

	// 모든 키의 상태를 None으로 초기화
	_states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::Update()
{
	// 전체 키보드의 현재 상태를 저장할 배열 (256개)
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	// 현재 키보드 상태를 가져온다 (실패 시 return)
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// 0~255 모든 키 검사
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 해당 키가 눌렸는지 확인 (최상위 비트가 1이면 눌린 상태)
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// 이전 프레임에서도 누르고 있었다면 Press
			if (state == KeyState::Press || state == KeyState::Down)
			{
				state = KeyState::Press;
			}
			// 이번 프레임에 처음 누른 경우 Down
			else
			{
				state = KeyState::Down;
			}
		}
		else
		{
			KeyState& state = _states[key];

			// 이전 프레임에는 누르고 있었으나 지금은 뗀 경우 → Up
			if (state == KeyState::Press || state == KeyState::Down)
			{
				state = KeyState::Up;
			}
			// 원래부터 누르지 않았던 경우 → None
			else
			{
				state = KeyState::None;
			}
		}
	}

	// 현재 마우스 위치를 클라이언트 좌표계로 가져옴
	::GetCursorPos(&_mousePos);               // 화면 좌표계로 마우스 위치 확인
	::ScreenToClient(_hwnd, &_mousePos);      // 클라이언트 기준 좌표로 변환
}
