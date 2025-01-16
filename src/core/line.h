#pragma once
#include "../types.h"

namespace core
{

    class cLine
    {

    public:
        cLine();
        ~cLine();

        bool Upload();
        void Render(int startVertex = 0, int numSegments = 0) const;
        void PushVertex(sPoint pt);
        void PopVertex();

        sLineFormat data;

    private:

        unsigned int _vao;
        unsigned int _vbo;
    };

}
