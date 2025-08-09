#include "pch.h"
#include "LineMesh.h"
#include <fstream>
#include "Utils.h"

// 라인 정보를 파일로 저장
void LineMesh::Save(wstring path)
{
	wofstream file;
	file.open(path);

	// 중심 좌표 계산을 위한 min/max
	int32 minX = INT32_MAX;
	int32 maxX = INT32_MIN;
	int32 minY = INT32_MAX;
	int32 maxY = INT32_MIN;

	for (auto& line : _lines)
	{
		POINT from = line.first;
		POINT to = line.second;

		minX = min(min(minX, from.x), to.x);
		maxX = max(max(maxX, from.x), to.x);
		minY = min(min(minY, from.y), to.y);
		maxY = max(max(maxY, from.y), to.y);
	}

	// 라인의 중심을 기준으로 정렬
	int32 midX = (maxX + minX) / 2;
	int32 midY = (maxY + minY) / 2;

	// 라인 개수 저장
	file << static_cast<int32>(_lines.size()) << endl;

	// 중심 기준으로 오프셋된 라인 저장
	for (auto& line : _lines)
	{
		POINT from = line.first;
		from.x -= midX;
		from.y -= midY;

		POINT to = line.second;
		to.x -= midX;
		to.y -= midY;

		wstring str = std::format(L"({0},{1})->({2},{3})", from.x, from.y, to.x, to.y);
		file << str << endl;
	}

	file.close();
}

// 파일로부터 라인 데이터를 불러옴
void LineMesh::Load(wstring path)
{
	wifstream file;
	file.open(path);

	// 라인 개수 읽기
	int32 count;
	file >> count;

	_lines.clear();

	// 기본 중심 좌표 보정값
	int32 midX = 400;
	int32 midY = 300;

	for (int32 i = 0; i < count; i++)
	{
		POINT pt1;
		POINT pt2;

		wstring str;
		file >> str;
		::swscanf_s(str.c_str(), L"(%d,%d)->(%d,%d)", &pt1.x, &pt1.y, &pt2.x, &pt2.y);

		_lines.push_back(make_pair(pt1, pt2));
	}

	file.close();
}

// 라인 데이터를 pos 위치를 기준으로 렌더링
void LineMesh::Render(HDC hdc, Pos pos) const
{
	for (auto& line : _lines)
	{
		POINT pt1 = line.first;
		POINT pt2 = line.second;

		Pos pos1;
		pos1.x = pos.x + (float)pt1.x;
		pos1.y = pos.y + (float)pt1.y;

		Pos pos2;
		pos2.x = pos.x + (float)pt2.x;
		pos2.y = pos.y + (float)pt2.y;

		Utils::DrawLine(hdc, pos1, pos2);
	}
}
