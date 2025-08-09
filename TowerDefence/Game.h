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
	// ������ �ڵ�
	HWND _hwnd = {};
	// Ŭ���̾�Ʈ ������ �׸��� ���� ����̽� ���ؽ�Ʈ
	HDC _hdc = {};

private:
	// Double Buffuring
	RECT _rect;
	HDC _hdcBack = {};
	HBITMAP _bmpBack = {};
};

