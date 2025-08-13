#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <windows.h>

// ȭ���� �� ä��� Ÿ��/�� ũ�� (800x600 = 40x20 by 40x15)
constexpr int TILE = 40;
constexpr int MAP_W = 20;
constexpr int MAP_H = 15;

// �׸��� ��ǥ
struct GridPos { int x; int y; };

// Ÿ�� �ǵ� (�� / ��ġ ��)
enum class TileType : uint8_t
{
    Path,       // ���� ��� (���� ��������Ʈ ���� ����, ���� �̹���/���� ���)
    BuildOnly,  // Ÿ�� ��ġ ����
    Blocker     // ��ġ �Ұ�(��ֹ�)
};

// GDI+ ���漱��
namespace Gdiplus { class Image; }

class Map
{
public:
    Map();
    ~Map();

    // �⺻ ���̾ƿ� ����
    void Init();

    // Ÿ�� �׸��� ����
    void Render(HDC hdc);

    // ��ǥ ��ƿ
    static bool  InBounds(int gx, int gy);
    static POINT GridToPixelCenter(int gx, int gy);
    static bool  PixelToGrid(int px, int py, int& gx, int& gy);

    // ���
    const std::vector<GridPos>& GetPath() const { return _path; }

    // ��ǥ�� �̹��� �������̵� API
    // relPath: "Assets\\Map\\myTile.png" ����
    void AddTileOverride(int gx, int gy, const std::wstring& relPath);
    void ClearTileOverrides();

private:
    void SetPathPolyline(const std::vector<GridPos>& polyline);
    void SetBlockers(); // ��ġ���� ���� �پ��� ��ֹ� ��ġ

    // ���ҽ� �ε� 
    void LoadTileOverrides();    // �������̵� ��ǥ �ϵ��ڵ� ���
    Gdiplus::Image* LoadImageCached(const std::wstring& relPath);

    // ���� Ű ��ƿ(��ǥ �� ���� Ű)
    static uint32_t Key(int gx, int gy) { return uint32_t(gy) * 1024u + uint32_t(gx); }

private:
    TileType    _tile[MAP_H][MAP_W] = {};
    std::vector<GridPos> _path;

    // ��ο� "����" ��������Ʈ �̹���(�ɼ�)
    Gdiplus::Image* _imgPathDefault = nullptr;

    // ��ǥ�� �̹��� �������̵�: Ư�� ��ǥ > �̹��� ������
    std::unordered_map<uint32_t, Gdiplus::Image*> _tileOverride;

    // ��� ���ڿ� > �̹��� ������ (�ߺ� �ε� ����)
    std::unordered_map<std::wstring, Gdiplus::Image*> _imageCache;
};
