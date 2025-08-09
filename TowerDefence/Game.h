#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	// 윈도우 핸들
	HWND _hwnd = {};
	// 클라이언트 영역에 그리기 위한 디바이스 컨텍스트
	HDC _hdc = {};

private:
	// Double Buffuring
	RECT _rect;
	HDC _hdcBack = {};
	HBITMAP _bmpBack = {};
};

