#pragma once

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
        void Begin();
        void End() { SwapBuffers(hdc); }

    private:
        int window_width = 0;
        int window_height = 0;

    private:
        HWND hwnd = nullptr;
        HGLRC hrc = nullptr;
        HDC hdc = nullptr;
    };

}
