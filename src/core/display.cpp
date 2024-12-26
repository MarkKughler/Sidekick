#include "display.h"
#include <iostream>

core::cDisplay::cDisplay() {}
core::cDisplay::~cDisplay() {}

LRESULT CALLBACK core::cDisplay::MessageRouter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

	case WM_SIZE:
		ogl.Reshape(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_DESTROY:
		std::cout << "--- NubExit ---" << std::endl;
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
	
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


bool core::cDisplay::Create(LPCSTR title, LPCSTR class_name, int width, int height)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = MessageRouter;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SIDEKICK));
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SIDEKICK_SM));
	wc.hCursor = LoadCursor(wc.hInstance, MAKEINTRESOURCE(IDC_CROSSHAIR));
	wc.hbrBackground = CreateSolidBrush(RGB(35, 35, 35));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = class_name;
	if (!RegisterClassEx(&wc)) return false;

	DWORD style = WS_OVERLAPPEDWINDOW;
	hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		class_name, title, style, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, wc.hInstance, this);

	ogl.Create(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

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

