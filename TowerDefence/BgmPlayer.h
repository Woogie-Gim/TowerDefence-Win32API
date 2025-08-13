#pragma once
#include <string>
#include <windows.h>

class BgmPlayer
{
public:
    BgmPlayer();
    ~BgmPlayer();

    // ���� ��� ���� (wav/mp3 ����)
    void SetSource(const std::wstring& path);

    // ���� ��� (�̹� ��� ���̸� ����)
    void PlayLoop();

    bool IsPlaying() const { return _playing; }

private:
    bool IsMp3() const;
    static std::wstring ToLower(std::wstring s);
    void ApplyVolumeAfterOpenMci();    // MP3�� (mci setaudio)

private:
    std::wstring _path;
    bool _playing = false;

    // mp3 ��� �� MCI ���
    bool        _useMci = false;
    std::wstring _mciAlias = L"bgm_player";

    float _volume = 0.5f;
};
