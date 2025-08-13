#include "pch.h"
#include "BgmPlayer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")   // PlaySound, mciSendString, waveOutSetVolume

BgmPlayer::BgmPlayer()
{

}

BgmPlayer::~BgmPlayer()
{
    // 공개 Stop은 없애도, 소멸 시엔 정리해 주는 게 안전
    if (_playing) 
    {
        if (_useMci) 
        {
            wchar_t cmd[256];
            swprintf(cmd, 256, L"stop %s", _mciAlias.c_str());   mciSendStringW(cmd, nullptr, 0, nullptr);
            swprintf(cmd, 256, L"close %s", _mciAlias.c_str());   mciSendStringW(cmd, nullptr, 0, nullptr);
        }
        else 
        {
            PlaySoundW(nullptr, 0, 0);
        }
    }
}

void BgmPlayer::SetSource(const std::wstring& path)
{
    // 재생 중 바꾸면 먼저 내부적으로 정지
    if (_playing) 
    {
        if (_useMci)
        {
            wchar_t cmd[256];
            swprintf(cmd, 256, L"stop %s", _mciAlias.c_str());   mciSendStringW(cmd, nullptr, 0, nullptr);
            swprintf(cmd, 256, L"close %s", _mciAlias.c_str());   mciSendStringW(cmd, nullptr, 0, nullptr);
        }
        else 
        {
            PlaySoundW(nullptr, 0, 0);
        }
        _playing = false;
        _useMci = false;
    }
    _path = path;
}

std::wstring BgmPlayer::ToLower(std::wstring s)
{
    for (auto& c : s) c = (wchar_t)towlower(c);
    return s;
}

bool BgmPlayer::IsMp3() const
{
    if (_path.size() < 4) return false;
    auto low = ToLower(_path);
    return low.rfind(L".mp3") == low.size() - 4;
}

void BgmPlayer::ApplyVolumeAfterOpenMci()
{
    // MP3: 0~1000 스케일
    int v = (int)(1000.0f * _volume);
    if (v < 0) v = 0; if (v > 1000) v = 1000;

    wchar_t cmd[256];
    swprintf(cmd, 256, L"setaudio %s volume to %d", _mciAlias.c_str(), v);
    mciSendStringW(cmd, nullptr, 0, nullptr);
}

void BgmPlayer::PlayLoop()
{
    if (_path.empty() || _playing) return;

    if (IsMp3())
    {
        _useMci = true;

        wchar_t cmd[1024];

        // 혹시 열려있으면 닫기
        swprintf(cmd, 1024, L"close %s", _mciAlias.c_str());
        mciSendStringW(cmd, nullptr, 0, nullptr);

        swprintf(cmd, 1024, L"open \"%s\" type mpegvideo alias %s", _path.c_str(), _mciAlias.c_str());
        if (mciSendStringW(cmd, nullptr, 0, nullptr) != 0) { _useMci = false; _playing = false; return; }

        // 볼륨 50%
        ApplyVolumeAfterOpenMci();

        swprintf(cmd, 1024, L"play %s repeat", _mciAlias.c_str());
        if (mciSendStringW(cmd, nullptr, 0, nullptr) != 0) 
        {
            swprintf(cmd, 1024, L"close %s", _mciAlias.c_str()); mciSendStringW(cmd, nullptr, 0, nullptr);
            _useMci = false; _playing = false; return;
        }

        _playing = true;
    }
}
