#pragma once
#include "ResourceBase.h"

// ���� ������(POINT �ֵ��� ����)�� �����ϰ� �׸� �� �ִ� ���ҽ� Ŭ����
class LineMesh : public ResourceBase
{
public:
	// ���� �����͸� ���Ϸ� ����
	void Save(wstring path);

	// ���Ͽ��� ���� �����͸� �ҷ���
	void Load(wstring path);

	// pos�� �������� ���ε��� ȭ�鿡 �׷���
	void Render(HDC hdc, Pos pos) const;

protected:
	// ������ �����ϴ� POINT �ֵ��� ����
	vector<pair<POINT, POINT>> _lines;
};
