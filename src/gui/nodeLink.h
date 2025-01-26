#pragma once

#include "../types.h"

namespace gui
{

    class cLink
    {
    public:
        cLink() {  }
    
        cLink(sPoint start, sPoint pt1, sPoint pt2, sPoint end);
        void TestHoverHandles(sPoint mouse_pos);

        std::array<float, 30> data = { 0 };  // format: x, y, r, g, b (6 vertices)
        int slot_id = -1;                    // location in container 
        int handle_id = -1;                  // selected handle (start, ctrl1, ctrl2, end)
        bool is_hover = false;
        bool is_dragging = false;

        // todo: from object
        // todo: to object
    };


    class cLinkContainer
    {
    public:
        void Initialize(int allocation_size);
        void Push(cLink &item);
        void Pop(int slot_id);
     
        void Render(sPoint mouse_pos, bool mouse_down);

        std::vector<cLink> links;    // cLink container
        int capacity = -1;           // max number of cLink objects before a memory resize required 

    private:
        unsigned int _vao = 0;
        unsigned int _vbo = 0;

        void UpdateSegmentColor(unsigned int id, int slot);
        void UpdateCtrlPtPos(unsigned int id, int slot, sPoint mouse_pos);
        void ResetLinkColor(unsigned int id);
    };

}
