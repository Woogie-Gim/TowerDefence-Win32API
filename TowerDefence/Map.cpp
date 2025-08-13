#include "pch.h"
#include "Map.h"
#include "GdiPlusSafe.h"
#include <filesystem>

extern std::wstring MakeAssetPath(const std::wstring& rel);

Map::Map()
{

}

Map::~Map()
{

}

void Map::Init()
{
    // 전부 설치 가능(BuildOnly)으로 시작
    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            _tile[y][x] = TileType::BuildOnly;
        }
    }

    // 800x600 “지그재그” 경로 (하드코딩)
    std::vector<GridPos> polyline;
    for (int x = 0; x <= 6; ++x) polyline.push_back({ x, 2 });
    for (int y = 3; y <= 7; ++y) polyline.push_back({ 6, y });
    for (int x = 7; x <= 12; ++x) polyline.push_back({ x, 7 });
    for (int y = 6; y >= 4; --y) polyline.push_back({ 12, y });
    for (int x = 13; x <= 19; ++x) polyline.push_back({ x, 4 });
    for (int y = 5; y <= 10; ++y) polyline.push_back({ 19, y });
    for (int x = 18; x >= 10; --x) polyline.push_back({ x, 10 });
    for (int y = 11; y <= 13; ++y) polyline.push_back({ 10, y });
    for (int x = 11; x <= 19; ++x) polyline.push_back({ x, 13 });

    SetPathPolyline(polyline);

    // 설치 불가 등 배치
    SetBlockers();

    // 좌표별 오버라이드 하드코딩 등록
    LoadTileOverrides();
}

void Map::SetPathPolyline(const std::vector<GridPos>& polyline)
{
    _path.clear();
    _path.reserve(polyline.size());

    for (const auto& gp : polyline)
    {
        if (!InBounds(gp.x, gp.y)) continue;
        _tile[gp.y][gp.x] = TileType::Path; // 경로는 설치 불가
        _path.push_back(gp);
    }
}

void Map::SetBlockers()
{
    auto place = [&](int gx, int gy)
    {
        if (!InBounds(gx, gy)) return;
        if (_tile[gy][gx] == TileType::Path) return; // 경로는 건드리지 않음
        _tile[gy][gx] = TileType::Blocker;
    };

    // 좌측 지역
    place(2, 1); place(3, 1); place(4, 1);
    place(1, 5); place(2, 5);

    // 중앙 상단/중단
    place(8, 1); place(9, 2);
    place(8, 6); place(9, 6); place(11, 6);

    // 중앙 하단
    place(7, 9); place(8, 9);
    place(9, 11); place(11, 11);

    // 우측 지역
    place(15, 3); place(16, 3);
    place(14, 8); place(15, 8); place(17, 8);
    place(13, 12); place(14, 12); place(16, 12);
}

void Map::LoadTileOverrides()
{
    auto AddHorizontalRange = [&](int y, int x1, int x2, const std::wstring& img)
    {
        if (x1 > x2) std::swap(x1, x2);
        for (int x = x1; x <= x2; ++x)
            AddTileOverride(x, y, img);
    };
    auto AddVerticalRange = [&](int x, int y1, int y2, const std::wstring& img)
    {
        if (y1 > y2) std::swap(y1, y2);
        for (int y = y1; y <= y2; ++y)
            AddTileOverride(x, y, img);
    };

    // Monster Path Sprite 적용
    AddHorizontalRange(2, 0, 5, L"Assets\\Map\\Path_Horizontal.png");
    AddTileOverride(6, 2, L"Assets\\Map\\path_R_Up_Corner.png");
    AddVerticalRange(6, 3, 6, L"Assets\\Map\\Path_Vertical.png");
    AddTileOverride(6, 7, L"Assets\\Map\\path_R_Down_Corner.png");

    AddHorizontalRange(7, 7, 11, L"Assets\\Map\\Path_Horizontal.png");
    AddTileOverride(12, 7, L"Assets\\Map\\path_L_Down_Corner.png");
    AddVerticalRange(12, 5, 6, L"Assets\\Map\\Path_Vertical.png");
    AddTileOverride(12, 4, L"Assets\\Map\\path_L_Up_Corner.png");

    AddHorizontalRange(4, 13, 18, L"Assets\\Map\\Path_Horizontal.png");
    AddTileOverride(19, 4, L"Assets\\Map\\path_R_Up_Corner.png");
    AddVerticalRange(19, 5, 9, L"Assets\\Map\\Path_Vertical.png");
    AddTileOverride(19, 10, L"Assets\\Map\\path_L_Down_Corner.png");

    AddHorizontalRange(10, 11, 18, L"Assets\\Map\\Path_Horizontal.png");
    AddTileOverride(10, 10, L"Assets\\Map\\path_L_Up_Corner.png");
    AddVerticalRange(10, 11, 12, L"Assets\\Map\\Path_Vertical.png");
    AddTileOverride(10, 13, L"Assets\\Map\\path_R_Down_Corner.png");

    AddHorizontalRange(13, 11, 19, L"Assets\\Map\\Path_Horizontal.png");

    // BuildOnly & Blocker 배치

    // BuildOnly 이미지
    const std::wstring buildImg = L"Assets\\Map\\BuildOnly.png";

    // Blocker 이미지 목록 (1~6)
    const std::wstring blockerImgs[6] = 
    {
        L"Assets\\Map\\Blocker1.png",
        L"Assets\\Map\\Blocker2.png", 
        L"Assets\\Map\\Blocker3.png",
        L"Assets\\Map\\Blocker4.png",
        L"Assets\\Map\\Blocker5.png", 
        L"Assets\\Map\\Blocker6.png" 
    };

    // Blocker 배치용 패턴 (한 번만 고정)
    int blockerPattern[MAP_H][MAP_W] = {};

    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            if (_tile[y][x] == TileType::BuildOnly)
            {
                AddTileOverride(x, y, buildImg);
            }
            else if (_tile[y][x] == TileType::Blocker)
            {
                // 0~5 인덱스 반복되게 배치
                int idx = ((x * 7 + y * 3) % 6);
                AddTileOverride(x, y, blockerImgs[idx]);
            }
        }
    }
}

Gdiplus::Image* Map::LoadImageCached(const std::wstring& relPath)
{
    auto it = _imageCache.find(relPath);
    if (it != _imageCache.end())
        return it->second;

    std::wstring full = MakeAssetPath(relPath);
    if (!std::filesystem::exists(full))
        return nullptr;

    Gdiplus::Image* img = Gdiplus::Image::FromFile(full.c_str());
    if (img && img->GetLastStatus() == Gdiplus::Ok)
    {
        _imageCache.emplace(relPath, img);
        return img;
    }
    delete img;
    return nullptr;
}

void Map::AddTileOverride(int gx, int gy, const std::wstring& relPath)
{
    if (!InBounds(gx, gy)) return;
    if (Gdiplus::Image* img = LoadImageCached(relPath))
        _tileOverride[Key(gx, gy)] = img;
}

void Map::ClearTileOverrides()
{
    _tileOverride.clear();
}

bool Map::InBounds(int gx, int gy)
{
    return (0 <= gx && gx < MAP_W && 0 <= gy && gy < MAP_H);
}

POINT Map::GridToPixelCenter(int gx, int gy)
{
    POINT p;
    p.x = gx * TILE + TILE / 2;
    p.y = gy * TILE + TILE / 2;
    return p;
}

bool Map::PixelToGrid(int px, int py, int& gx, int& gy)
{
    if (px < 0 || py < 0) return false;
    gx = px / TILE;
    gy = py / TILE;
    return InBounds(gx, gy);
}

void Map::Render(HDC hdc)
{
    // GDI+ 컨텍스트
    Gdiplus::Graphics g(hdc);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);

    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            RECT r{ x * TILE, y * TILE, (x + 1) * TILE, (y + 1) * TILE };
            Gdiplus::Rect dest(r.left, r.top, TILE, TILE);

            // 좌표별 오버라이드 이미지가 있으면 그것을 최우선으로 그림
            auto it = _tileOverride.find(Key(x, y));
            if (it != _tileOverride.end() && it->second)
            {
                g.DrawImage(it->second, dest);
            }
            else
            {
                // 오버라이드가 없으면, 타일 타입에 따라 처리
                if (_tile[y][x] == TileType::Path)
                {
                    // 경로는 하나의 기본 이미지(있으면) 혹은 임시 색
                    if (_imgPathDefault)
                        g.DrawImage(_imgPathDefault, dest);
                    else
                    {
                        HBRUSH b = CreateSolidBrush(RGB(180, 140, 90)); // 갈색 톤
                        FillRect(hdc, &r, b);
                        DeleteObject(b);
                    }
                }
                else
                {
                    // Path가 아닌 타일은 기존 색 칠하기
                    const COLORREF fill =
                        (_tile[y][x] == TileType::Blocker) ? RGB(110, 110, 110) :
                        (_tile[y][x] == TileType::BuildOnly) ? RGB(150, 210, 150) :
                        RGB(90, 160, 90); // Empty 등

                    HBRUSH b = CreateSolidBrush(fill);
                    FillRect(hdc, &r, b);
                    DeleteObject(b);
                }
            }
        }
    }
}
