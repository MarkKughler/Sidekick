#pragma once
#include "../core/line.h"

namespace gui
{

    class cBSpline
    {

    public:
        cBSpline();
        ~cBSpline();

        bool Create(sPoint start, sPoint end);
        void Render();

    private:
        std::vector<sPoint> GeneratePoints(int k, int numSegments);
        sPoint EvaluateBSpline(int k, float t, const std::vector<float> &knots);
        float BSplineBasis(int i, int k, float t, const std::vector<float> &knots);
        
        core::cLine _spline;
        sPoint _controlPoints[4];
        
    };

}
