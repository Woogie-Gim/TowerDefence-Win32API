#include "pch.h"
#include "BgmPlayer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")   // PlaySound, mciSendString, waveOutSetVolume

BgmPlayer::BgmPlayer()
{

}

BgmPlayer::~BgmPlayer()
{
    // ���� Stop�� ���ֵ�, �Ҹ� �ÿ� ������ �ִ� �� ����
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
    // ��� �� �ٲٸ� ���� ���������� ����
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
    // MP3: 0~1000 ������
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

        // Ȥ�� ���������� �ݱ�
        swprintf(cmd, 1024, L"close %s", _mciAlias.c_str());
        mciSendStringW(cmd, nullptr, 0, nullptr);

        swprintf(cmd, 1024, L"open \"%s\" type mpegvideo alias %s", _path.c_str(), _mciAlias.c_str());
        if (mciSendStringW(cmd, nullptr, 0, nullptr) != 0) { _useMci = false; _playing = false; return; }

        // ���� 50%
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
