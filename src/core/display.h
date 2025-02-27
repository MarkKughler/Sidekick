#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN_32_EXTRA_LEAN
#include <Windows.h>
#include "../resource/res.h"
#include "../lib/glad/glad_4_6.h"
#include "ogl.h"

namespace core
{

	class cDisplay
	{
	public:
		cDisplay();
		~cDisplay();

		bool Create(LPCSTR title, LPCSTR class_name, sConfiguration* config);

		static LRESULT CALLBACK MessageRelay(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		HWND hWnd = 0;
		cOGLContext ogl;

		int input_state[256] = { 0 };
		bool lButtonDown = false;
		bool window_dragging = false;

	private:
		sConfiguration* pConfig;
		HCURSOR cursor_crosshair;
		HCURSOR cursor_arrow;
	};
	
}
