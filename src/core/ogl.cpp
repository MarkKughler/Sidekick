
#include <iostream>
#include "display.h"

#include "../lib/glad/glad_4_6.h"
#define WGL_CONTEXT_MAJOR_VERSION 0x2091
#define WGL_CONTEXT_MINOR_VERSION 0X2092
#define WGL_CONTEXT_FLAGS 0X2094
#define WGL_CONTEXT_COREPROFILE_BIT 0x00000001
#define WGL_CONTEXT_PROFILE_MASK 0x9126
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);
typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);

#pragma comment(lib, "opengl32.lib")

core::cOGLContext::cOGLContext() {};
core::cOGLContext::cOGLContext(HWND in_hwnd) { Create(in_hwnd); }


core::cOGLContext::~cOGLContext()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hrc);
    ReleaseDC(hwnd, hdc);
}


bool core::cOGLContext::Create(HWND in_hwnd)
{
    hwnd = in_hwnd;
    hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixel_format = ChoosePixelFormat(hdc, &pfd);
    if (pixel_format == 0) return false;
    if (!SetPixelFormat(hdc, pixel_format, &pfd)) return false;

    HGLRC ogl_rc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, ogl_rc);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = nullptr;
    wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    const int attribute_list[] = {
        WGL_CONTEXT_MAJOR_VERSION, 4,
        WGL_CONTEXT_MINOR_VERSION, 6,
        WGL_CONTEXT_FLAGS, 0,
        WGL_CONTEXT_PROFILE_MASK,
        WGL_CONTEXT_COREPROFILE_BIT, 0,
    };
    HGLRC hglrc = wglCreateContextAttribs(hdc, 0, attribute_list);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(ogl_rc);
    wglMakeCurrent(hdc, hglrc);
    if (!gladLoadGL()) std::cout << "Could not initialize GLAD" << std::endl;
    else std::cout << "Using OpenGL version: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    bool swapControlSupported = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;
    // https://www.khronos.org/opengl/wiki/Swap_Interval
    int vsynch = 0;
    if (swapControlSupported) {
        PFNWGLSWAPINTERVALEXTPROC wglSwapInternalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
        if (wglSwapInternalEXT(1)) std::cout << "VSync enabled" << std::endl;
        else std::cout << "Could not enable VSync" << std::endl;
    }
    else
        std::cout << "WGL_EXT_swap_control not supported" << std::endl;

    return true;
}


void core::cOGLContext::SetState()
{
    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}


void core::cOGLContext::Reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}


void core::cOGLContext::Begin()
{
    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
