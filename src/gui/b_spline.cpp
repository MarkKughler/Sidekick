#include "b_spline.h"


gui::cBSpline::cBSpline()
{
    _controlPoints[0] = { 0, 0 };
    _controlPoints[1] = { 0, 0 };
    _controlPoints[2] = { 0, 0 };
    _controlPoints[3] = { 0, 0 };
}

gui::cBSpline::~cBSpline()
{

}


bool gui::cBSpline::Create(sPoint start, sPoint end)
{
    float offsetX = (end.x - start.x) * 0.33f;
    float offsetY = (end.y - start.y) * 0.33f;
    _controlPoints[0] = start;
    _controlPoints[1] = sPoint(start.x + offsetX, start.y - offsetY); // hack start
    _controlPoints[2] = sPoint(end.x - offsetX, end.y + offsetY);
    _controlPoints[3] = end;

    _spline.data.stride = 2;
    _spline.data.num_verts = 50;
    std::vector<sPoint> points = GeneratePoints(3, (int)_spline.data.num_verts-1);
    for (sPoint pt : points)
    {
        _spline.data.vdata.push_back(pt.x);
        _spline.data.vdata.push_back(pt.y);
    }
    _spline.Upload();
    return true;
}


void gui::cBSpline::Render()
{
    _spline.Render();
}


std::vector<sPoint> gui::cBSpline::GeneratePoints(int k, int numSegments)
{

    // fuck it for tonight... spline setup started
    std::vector<sPoint> pts;
    //std::vector<float> knots = GenerateKnot WTF google foo bitch
    pts.push_back(_controlPoints[0]);
    pts.push_back(_controlPoints[1]);
    pts.push_back(_controlPoints[2]);
    pts.push_back(_controlPoints[3]);
    return pts;
}


sPoint gui::cBSpline::EvaluateBSpline(int k, float t, const std::vector<float>& knots)
{
    sPoint pt = { 0, 0 };
    return pt;
}


float gui::cBSpline::BSplineBasis(int i, int k, float t, const std::vector<float>& knots)
{
    return 0;
}
