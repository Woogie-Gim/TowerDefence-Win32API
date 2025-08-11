#pragma once

// MSVC: std::byte 비활성화해서 Windows RPC의 'byte'와 충돌 방지
#ifndef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <cstdint>
#include <cstdlib>
#include <crtdbg.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <format>
#include <filesystem>

// 프로젝트 공용
#include "Types.h"
#include "Defines.h"
#include "Enums.h"

// ★ 프로젝트가 std:: 접두어 없이 많이 쓰고 있으므로, 여기서만 허용
using namespace std;

// 디버그 메모리릭 체크
#ifdef _DEBUG
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif