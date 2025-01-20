#include "display.h"
#include "../nublog.h"
#include <iostream>

core::cDisplay::cDisplay() {}
core::cDisplay::~cDisplay() {}

LRESULT CALLBACK core::cDisplay::MessageRelay(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	cDisplay* pDisplay;
	if (msg == WM_CREATE) {
		pDisplay = (cDisplay*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pDisplay);
	}
	else
		pDisplay = (cDisplay*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	return pDisplay->WndProc(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK core::cDisplay::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_MOUSEMOVE:
		if (HIWORD(lParam) < 24) SetCursor(cursor_arrow);
		else SetCursor(cursor_crosshair);
		break;

	case WM_LBUTTONDOWN:
		lButtonDown = true;
		break;
	case WM_LBUTTONUP:
		window_dragging = false;
		break;
	
	case WM_SIZE:
		pConfig->screen.x = LOWORD(lParam);
		pConfig->screen.y = HIWORD(lParam);
		ogl.Reshape(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_DESTROY:
		LOG_INFO("core::cDisplay::WndProc", "===> Exiting Application");
		CloseWindow(hwnd);
		PostQuitMessage(0);
		break;

	case WM_KEYUP:
		input_state[(int)wParam] = 0;

		// todo: move to assign-able function pointer
		// todo: separate by keys
		switch (wParam) {
		case VK_ESCAPE:
			static bool bWireframe = false;
			if (bWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			bWireframe = !bWireframe;
			break;
		}

		break;

	case WM_KEYDOWN:
		input_state[(int)wParam] = 1;
		break;

	case WM_NCPAINT:
	case WM_ERASEBKGND:
		return 0;

	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


bool core::cDisplay::Create(LPCSTR title, LPCSTR class_name, sConfiguration* config)
{
	pConfig = config;
	HINSTANCE instance = GetModuleHandle(0);
	cursor_crosshair = LoadCursor(instance, MAKEINTRESOURCE(IDC_CROSSHAIR));
	cursor_arrow = LoadCursor(instance, MAKEINTRESOURCE(IDC_POINTER));

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = MessageRelay;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_SIDEKICK));
	wc.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(IDI_SIDEKICK_SM));
	wc.hCursor = cursor_crosshair;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = class_name;
	if (!RegisterClassEx(&wc)) return false;
	
	DWORD style = WS_POPUP | WS_VISIBLE;
	DWORD style_ex = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	hWnd = CreateWindowEx(style_ex, class_name, title, style, CW_USEDEFAULT, CW_USEDEFAULT, config->screen.x, config->screen.y, nullptr, nullptr, instance, this);
	//UpdateWindow(hWnd);
	//ShowWindow(hWnd, SW_SHOWNORMAL);
	ogl.Create(hWnd);
	

	return true;
}





/*
RECT core::getRectWorkarea()
{
	RECT rect_screen = {};
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect_screen, 0);

	if (rect_screen.top != 0) rect_screen.bottom -= rect_screen.top;
	if (rect_screen.left != 0) rect_screen.right -= rect_screen.left;
	// todo start bar top
	// todo start bar left
	return rect_screen;
}
*/

