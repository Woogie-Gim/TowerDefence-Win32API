#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <windows.h>

// 화면을 꽉 채우는 타일/맵 크기 (800x600 = 40x20 by 40x15)
constexpr int TILE = 40;
constexpr int MAP_W = 20;
constexpr int MAP_H = 15;

// 그리드 좌표
struct GridPos { int x; int y; };

// 타일 의도 (길 / 설치 등)
enum class TileType : uint8_t
{
    Path,       // 몬스터 경로 (이제 스프라이트 판정 없음, 단일 이미지/색만 사용)
    BuildOnly,  // 타워 설치 가능
    Blocker     // 설치 불가(장애물)
};

// GDI+ 전방선언
namespace Gdiplus { class Image; }

class Map
{
public:
    Map();
    ~Map();

    // 기본 레이아웃 구성
    void Init();

    // 타일 그리드 렌더
    void Render(HDC hdc);

    // 좌표 유틸
    static bool  InBounds(int gx, int gy);
    static POINT GridToPixelCenter(int gx, int gy);
    static bool  PixelToGrid(int px, int py, int& gx, int& gy);

    // 경로
    const std::vector<GridPos>& GetPath() const { return _path; }

    // 좌표별 이미지 오버라이드 API
    // relPath: "Assets\\Map\\myTile.png" 형태
    void AddTileOverride(int gx, int gy, const std::wstring& relPath);
    void ClearTileOverrides();

private:
    void SetPathPolyline(const std::vector<GridPos>& polyline);
    void SetBlockers(); // 설치구역 위에 다양한 장애물 배치

    // 리소스 로드 
    void LoadTileOverrides();    // 오버라이드 좌표 하드코딩 등록
    Gdiplus::Image* LoadImageCached(const std::wstring& relPath);

    // 내부 키 유틸(좌표 → 정수 키)
    static uint32_t Key(int gx, int gy) { return uint32_t(gy) * 1024u + uint32_t(gx); }

private:
    TileType    _tile[MAP_H][MAP_W] = {};
    std::vector<GridPos> _path;

    // 경로용 "단일" 스프라이트 이미지(옵션)
    Gdiplus::Image* _imgPathDefault = nullptr;

    // 좌표별 이미지 오버라이드: 특정 좌표 > 이미지 포인터
    std::unordered_map<uint32_t, Gdiplus::Image*> _tileOverride;

    // 경로 문자열 > 이미지 포인터 (중복 로딩 방지)
    std::unordered_map<std::wstring, Gdiplus::Image*> _imageCache;
};
