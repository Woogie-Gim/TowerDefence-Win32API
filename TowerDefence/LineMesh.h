#pragma once
#include "ResourceBase.h"

// 라인 데이터(POINT 쌍들의 벡터)를 저장하고 그릴 수 있는 리소스 클래스
class LineMesh : public ResourceBase
{
public:
	// 라인 데이터를 파일로 저장
	void Save(wstring path);

	// 파일에서 라인 데이터를 불러옴
	void Load(wstring path);

	// pos를 기준으로 라인들을 화면에 그려줌
	void Render(HDC hdc, Pos pos) const;

protected:
	// 라인을 구성하는 POINT 쌍들의 벡터
	vector<pair<POINT, POINT>> _lines;
};
