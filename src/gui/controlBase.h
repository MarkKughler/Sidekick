#pragma once

namespace gui
{

    class cCtrlBase
    {

    public:

        cCtrlBase()
        {
            is_enabled  = true;
            is_pressed  = false;
            is_down     = false;
            is_released = false;
        }

        virtual ~cCtrlBase() { }

        void Enable()  { is_enabled = true;  }
        void Disable() { is_enabled = false; }

        virtual void Draw() { }

    protected:

        bool is_enabled;
        bool is_pressed;
        bool is_down;
        bool is_released;

    private:



    };

}