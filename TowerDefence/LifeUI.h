#pragma once

#include <windows.h>

class LifeUI
{
public:
	void Init(POINT pos, int maxLife = 20)
	{
		_pos = pos;
		_max = maxLife;
		_life = maxLife;
	}

	void Restart()
	{
		_life = _max;
	}

	void SetLife(int v)
	{
		_life = (v < 0 ? 0 : (v > _max ? _max : v));
	}

	void Add(int delta)
	{
		SetLife(_life + delta);
	}

	void Sub(int delta)
	{
		SetLife(_life - delta);
	}

	int GetLife() const { return _life; }
	bool IsDead() const { return _life <= 0; }

	// ���� ǥ�� : Life : nn + ���� ���� 20ĭ �� ����ġ�� ä��
	void Render(HDC hdc) const
	{
		int x = _pos.x;
		int y = _pos.y;

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		const wchar_t* label = L"LIFE :";
		TextOutW(hdc, x, y, label, lstrlenW(label));

		// ����
		wchar_t buf[32];
		wsprintfW(buf, L"%d", _life);
		TextOutW(hdc, x + 60, y, buf, lstrlenW(buf));

		// ��: 20ĭ ����Ʈ(���� 1px)
		const int cellW = 10;
		const int cellH = 10;
		const int gap = 1;
		const int step = cellW + gap;

		int bx = x;
		int by = y + 18;

		for (int i = 0; i < _max; ++i)
		{
			int left = bx + i * step;
			RECT r{ left, by, left + cellW, by + cellH };

			HBRUSH b = CreateSolidBrush(i < _life ? RGB(220, 60, 60) : RGB(40, 40, 40));
			FillRect(hdc, &r, b);
			DeleteObject(b);

			// �׵θ� ���(����): �ʹ� �����Ǹ� �ּ�ó��
			FrameRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}
	}

private:
	POINT _pos{ 10, 10 };
	int _max = 20;
	int _life = 20;
};