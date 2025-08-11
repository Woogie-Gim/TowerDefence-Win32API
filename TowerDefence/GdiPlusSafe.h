// GdiPlusSafe.h
#pragma once
#include <windows.h>
#include <objidl.h>  // IStream

// pch.h���� ����׿� 'new' ��ũ�ΰ� ���ǵǾ� ������ GDI+�� ���� �� ��� ����
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
