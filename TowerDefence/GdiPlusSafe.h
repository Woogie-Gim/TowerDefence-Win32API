// GdiPlusSafe.h
#pragma once
#include <windows.h>
#include <objidl.h>  // IStream

// pch.h에서 디버그용 'new' 매크로가 정의되어 있으면 GDI+가 깨짐 → 잠깐 해제
#ifdef new
#pragma push_macro("new")
#undef new
#define __RESTORE_NEW_MACRO__
#endif

#include <gdiplus.h>

#ifdef __RESTORE_NEW_MACRO__
#pragma pop_macro("new")
#undef __RESTORE_NEW_MACRO__
#endif

#pragma comment(lib, "gdiplus.lib")
