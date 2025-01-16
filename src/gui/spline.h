#pragma once
#include "../core/line.h"

namespace gui
{

    class cSpline
    {

    public:
        cSpline();
        cSpline(sPoint start, sPoint end);
        ~cSpline();

        bool Create(sPoint start, sPoint end);
        void Render();

    private:
        void GenerateSegments();
        
        core::cLine _spline;
        sPoint _controlPoints[5];
        
    };

}
