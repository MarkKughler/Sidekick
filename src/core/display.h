#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Style
{
	enum eStyle
	{
		parent,
		ctrl,
		num_styles
	};
}

namespace Core
{
	ATOM registerWindowClass(LPCSTR wndClassName, WNDPROC wndProc, int wndExtra = 0);
	HWND createWindowObject(HWND hParent, LPCSTR wndclassName, Style::eStyle style, DWORD ws_style, const char* title, RECT rect);
	RECT getRectWorkarea();

	static DWORD parent_ex_style = WS_EX_OVERLAPPEDWINDOW;
	static DWORD parent_style = WS_SYSMENU | WS_VISIBLE | WS_VSCROLL;
}
