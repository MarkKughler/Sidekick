#include "spline.h"


gui::cSpline::cSpline()
{
    _controlPoints[0] = { 0, 0 }; // start
    _controlPoints[1] = { 0, 0 }; // start run out
    _controlPoints[2] = { 0, 0 }; // center pivot
    _controlPoints[3] = { 0, 0 }; // end run in
    _controlPoints[4] = { 0, 0 }; // end
    _spline.data.num_verts = 0;
}

gui::cSpline::cSpline(sPoint start, sPoint end)
{
    Create(start, end);
}

gui::cSpline::~cSpline()
{

}


bool gui::cSpline::Create(sPoint start, sPoint end)
{
    _controlPoints[0] = start;
    _controlPoints[1] = sPoint(start.x + 4, start.y);
    _controlPoints[2] = sPoint(start.x + ((end.x-start.x)/2), start.y + ((end.y-start.y)/2));
    _controlPoints[3] = sPoint(end.x - 4, end.y);
    _controlPoints[4] = end;
    GenerateSegments();
    _spline.Upload();
    return true;
}


void gui::cSpline::Render()
{
    _spline.Render();
}


void gui::cSpline::GenerateSegments()
{
    sPoint p1 = { _controlPoints[1].x, _controlPoints[1].y };
    sPoint p2 = { _controlPoints[2].x, _controlPoints[2].y };
    float dx = (p2.x > p1.x) ? (p2.x - p1.x)/20.f : -(p1.x - p2.x)/20.f;
    float dy = (p2.y > p1.y) ? (p2.y - p1.y)/20.f : -(p1.y - p2.y)/20.f;
    //float dx = (p2.x - p1.x) / 20;
    //float dy = (p2.y - p1.y) / 20;
    _spline.data.vdata.push_back(_controlPoints[0]);
    _spline.data.vdata.push_back(_controlPoints[1]);
    for (int i = 0; i < 20; i++)
    {
        p1.x += dx;
        p1.y += dy * (i * 0.1f);
        _spline.data.vdata.push_back(p1);
    }
    for (int i = 20; i > 0; i--)
    {
        p1.x += dx;
        p1.y += dy * (i * 0.1f);
        _spline.data.vdata.push_back(p1);
    }
    _spline.data.vdata.push_back(_controlPoints[4]);
}

