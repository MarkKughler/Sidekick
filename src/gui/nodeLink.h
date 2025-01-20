#pragma once

#include "../types.h"


namespace gui
{

    class cLink
    {
    public:
        cLink(sPoint start, sPoint pt1, sPoint pt2, sPoint end);

        void Push();
        void Pop();

        sPoint vdata[6];  // vertex data
        sRect  bounds;    // bounding rectangle
    };


    class cLinkContainer
    {
    public:
        void Initialize();
        void Add(cLink);
        void Render(unsigned int color_loc);

        std::vector<cLink> links;

    private:
       // void CalculateIndexData(int);

        unsigned int _vao = 0;
        unsigned int _vbo = 0;
    };

}
