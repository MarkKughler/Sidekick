#pragma once
#include "nodeLink.h"
#include "../core/model.h"



namespace gui
{
    struct sNodeType
    {

    };

    class cNode
    {

    public:
        cNode(sPoint pos, int type);
        void TestHoverHandles(sPoint mouse_pos);

        
    private:
        core::cModel model;

        std::vector<int> input;
        std::vector<int> output;

        // todo: callback types

        sRect bounds;
    };


    class cNodeContainer
    {

    public:
        void Initialize(int allocation_size);
        void AddNode(sPoint mouse_pos);
        void RemoveNode(int id);
        void Render(sPoint mouse_pos, bool mouse_down);

        std::vector<cNode> nodes;  // cNode container
        
    };

}
