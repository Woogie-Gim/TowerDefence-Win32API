#pragma once

// 사용하고자 하는 키의 종류를 정의
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

// 키의 상태를 나타내는 열거형
enum class KeyState
{
	None,   // 아무 동작 없음
	Press,  // 누르고 있음
	Down,   // 이번 프레임에서 처음 눌림
	Up,     // 이번 프레임에서 처음 떼짐

	End     // 상태 개수 (반복문 등에서 사용)
};

// 배열 크기 정의용 상수
enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX) + 1,           // 키 코드 최대 수 (0~255)
	KEY_STATE_COUNT = static_cast<int32>(KeyState::End)           // 키 상태 종류 수
};

class InputManager
{
	DECLARE_SINGLE(InputManager); // 싱글톤 매크로

public:
	// 초기화: 윈도우 핸들을 저장
	void Init(HWND hwnd);
	// 매 프레임 호출하여 키 상태 갱신
	void Update();
	// 키를 누르고 있는 중인가?
	bool GetButton(KeyType key) { return GetState(key) == KeyState::Press; }
	// 키를 이번 프레임에 눌렀는가?
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }
	// 키를 이번 프레임에 떼었는가?
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }

	// 마우스 현재 위치 반환
	POINT GetMousePos() { return _mousePos; }

private:
	// 특정 키의 현재 상태를 반환
	KeyState GetState(KeyType key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd = 0;                  // 입력을 받을 윈도우 핸들
	vector<KeyState> _states;       // 전체 키의 상태를 저장하는 배열 (256개)
	POINT _mousePos;                // 마우스 위치
};
