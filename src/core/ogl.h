#pragma once
#include <Windows.h>
#include "../types.h"

namespace core
{

    class cOGLContext
    {
    public:
        cOGLContext();
        cOGLContext(HWND in_hwnd);
        ~cOGLContext();

        bool Create(HWND in_hwnd);
        void SetState();
        void Reshape(int width, int height);
        void Begin() const;
        void End() const;

        void GetOrtho(sMatrix&);

    private:
        int window_width = 0;
        int window_height = 0;
        int screen_width = 0;
        int screen_height = 0;

    private:
        HWND hwnd = nullptr;
        HGLRC hrc = nullptr;
        HDC hdc = nullptr;
    };

}
