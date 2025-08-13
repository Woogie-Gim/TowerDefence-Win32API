#pragma once
#include <string>
#include <windows.h>

class BgmPlayer
{
public:
    BgmPlayer();
    ~BgmPlayer();

    // 파일 경로 설정 (wav/mp3 지원)
    void SetSource(const std::wstring& path);

    // 루프 재생 (이미 재생 중이면 무시)
    void PlayLoop();

    bool IsPlaying() const { return _playing; }

private:
    bool IsMp3() const;
    static std::wstring ToLower(std::wstring s);
    void ApplyVolumeAfterOpenMci();    // MP3용 (mci setaudio)

private:
    std::wstring _path;
    bool _playing = false;

    // mp3 재생 시 MCI 사용
    bool        _useMci = false;
    std::wstring _mciAlias = L"bgm_player";

    float _volume = 0.5f;
};
