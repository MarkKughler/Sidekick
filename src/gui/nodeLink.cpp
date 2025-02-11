#include "../lib/glad/glad_4_6.h"
#include "../nublog.h"
#include "nodeLink.h"
#include "array"


gui::cLink::cLink(sPoint start, sPoint ctrl_pt1, sPoint ctrl_pt2, sPoint end)
{
    sPoint start_tail = { start.x + (ctrl_pt1.x-start.x), start.y };
    sPoint end_tail   = {   end.x - (end.x-ctrl_pt2.x),     end.y };
    
    data = {     
                  start.x,      start.y, 0.7f, 0.7f, 0.7f,
             start_tail.x, start_tail.y, 0.7f, 0.7f, 0.7f,
               ctrl_pt1.x,   ctrl_pt1.y, 0.7f, 0.7f, 0.7f,
               ctrl_pt2.x,   ctrl_pt2.y, 0.7f, 0.7f, 0.7f,
               end_tail.x,   end_tail.y, 0.7f, 0.7f, 0.7f,
                    end.x,        end.y, 0.7f, 0.7f, 0.7f 
           };
}

void gui::cLink::TestHoverHandles(sPoint mouse_pos)
{
    sPoint pt = mouse_pos;
    sRect r0 = {  int(data[0] - 5),  int(data[1] - 5),  int(data[0] + 5),  int(data[1] + 5) }; // start point bounding
    sRect r1 = { int(data[10] - 5), int(data[11] - 5), int(data[10] + 5), int(data[11] + 5) }; // control point bounding (start side)
    sRect r2 = { int(data[15] - 5), int(data[16] - 5), int(data[15] + 5), int(data[16] + 5) }; // control point bounding (end side)
    sRect r3 = { int(data[25] - 5), int(data[26] - 5), int(data[25] + 5), int(data[26] + 5) }; // end point bounding

    if ((pt.x > r0.x) && (pt.x < r1.x) && (pt.y > r0.y) && (pt.y < r0.h)) { handle_id = 1; return; }
    if ((pt.x > r1.x) && (pt.x < r1.w) && (pt.y > r1.y) && (pt.y < r1.h)) { handle_id = 2; return; }
    if ((pt.x > r2.x) && (pt.x < r2.w) && (pt.y > r2.y) && (pt.y < r2.h)) { handle_id = 3; return; }
    if ((pt.x > r2.x) && (pt.x < r3.w) && (pt.y > r3.y) && (pt.y < r3.h)) { handle_id = 4; return; }
    
    sPoint p4 = { data[10] + (data[15] - data[10])/2, data[11] + (data[16] - data[11])/2 };
    sRect r4 = { int(p4.x - 10), int(p4.y - 5), int(p4.x + 10), int(p4.y + 5) };
    if ((pt.x > r4.x) && (pt.x < r4.w) && (pt.y > r4.y) && (pt.y < r4.h)) { handle_id = 5; return; }
    
    handle_id = 0;
}


void gui::cLinkContainer::Initialize(int allocation_size)
{
    capacity = allocation_size;
    int vbuffer_size = sizeof(float) * 30; // (x, y, r, g, b) * 6 vertices per link
    vbuffer_size *= capacity;              // pre allocate a larger space for (N) links 

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vbuffer_size, nullptr, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float)*5, (void*)0);                   // x,y
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float)*5, (void*)(sizeof(float) * 2)); // r,g,b
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // the new drag link lives in container slot zero
    cLink new_link_data({ 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f });
    Push(new_link_data);
    is_dragging_new_link = false;
}

void gui::cLinkContainer::BeginNewLink(sPoint start)
{
    links[0].data[0] = start.x;
    links[0].data[1] = start.y;
    links[0].data[5] = start.x;
    links[0].data[6] = start.y;

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2, &links[0].data[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 5, sizeof(float) * 2, &links[0].data[5]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    is_dragging_new_link = true;
}


void gui::cLinkContainer::ResizeGpuBuffer()
{
    LOG_WARN("gui::cLinkContainer::ResizeGpuBuffer", "Not implemented");
}


void gui::cLinkContainer::Push(cLink &item)
{
    
    if (links.size() >= capacity) { LOG_WARN("gui::cLinkContainer::Add", "Max node links reached. Implement resize gpu buffer"); return; }

    item.slot_id = links.size();
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (links.size()*30), sizeof(float) * 30, &item.data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    links.push_back(item);
}


void gui::cLinkContainer::Pop(int slot)
{
    LOG_WARN("gui::cLinkContainer::Pop", "not implemented");
}


void gui::cLinkContainer::UpdateSegmentColor(unsigned int id, int slot)
{
    sRGB hover_color = { 0.0f, 0.5f, 1.0f };
    unsigned long long offset = (unsigned long long)30 * id;
    unsigned long long block_width = sizeof(float) * 3;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    switch (slot)
    {
    case 1:
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 2), block_width, &hover_color);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 7), block_width, &hover_color);
        break;
    case 2:
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 12), block_width, &hover_color);
        break;
    case 3:
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 17), block_width, &hover_color);
        break;
    case 4:
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 22), block_width, &hover_color);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 27), block_width, &hover_color);
        break;
    case 5:
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 12), block_width, &hover_color);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 17), block_width, &hover_color);
        break;        
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void gui::cLinkContainer::UpdateCtrlPtPos(unsigned int id, int slot, sPoint mouse_pos)
{
    sPoint new_pos = { (float)mouse_pos.x, (float)mouse_pos.y };
    sPoint new_pos2;
    unsigned long long offset = (unsigned long long)30 * id;
    unsigned long long block_width = sizeof(float) * 2;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    switch (slot)
    {
    case 1:
        new_pos2 = { new_pos.x + 20, new_pos.y };
        links[id].data[0] = new_pos.x;
        links[id].data[1] = new_pos.y;
        links[id].data[5] = new_pos2.x;
        links[id].data[6] = new_pos2.y;
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 0), block_width, &new_pos);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 5), block_width, &new_pos2);
        break;
    case 2:
        new_pos2 = { new_pos.x, links[id].data[1] };
        links[id].data[10] = new_pos.x;
        links[id].data[11] = new_pos.y;
        links[id].data[5] = new_pos2.x;
        links[id].data[6] = new_pos2.y;
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 10), block_width, &new_pos);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 5), block_width, &new_pos2);
        break;
    case 3:
        new_pos2 = { new_pos.x, links[id].data[26] };
        links[id].data[15] = new_pos.x;
        links[id].data[16] = new_pos.y;
        links[id].data[20] = new_pos2.x;
        links[id].data[21] = new_pos2.y;
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 15), block_width, &new_pos);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 20), block_width, &new_pos2);
        break;
    case 4:
        new_pos2 = { new_pos.x - 20, new_pos.y };
        links[id].data[25] = new_pos.x;
        links[id].data[26] = new_pos.y;
        links[id].data[20] = new_pos2.x;
        links[id].data[21] = new_pos2.y;
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 25), block_width, &new_pos);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 20), block_width, &new_pos2);
        break;
    case 5:
        links[id].data[11] = links[id].data[11] + (new_pos.y - links[id].data[11]);
        links[id].data[16] = links[id].data[16] + (new_pos.y - links[id].data[16]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 11), sizeof(float), &links[id].data[11]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (offset + 16), sizeof(float), &links[id].data[16]);
        break;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void gui::cLinkContainer::ResetLinkColor(unsigned int id)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (static_cast<unsigned long long>(id * 30)), sizeof(float) * 30, &links[id].data[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


sRect test_input = { 549, 300, 559, 320 };

void gui::cLinkContainer::Render(sPoint mouse_pos, bool mouse_down)
{
    glBindVertexArray(_vao);
    
    
    if (is_dragging_new_link)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        links[0].data[5] = mouse_pos.x;
        links[0].data[6] = mouse_pos.y;
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 5, sizeof(float) * 2, &links[0].data[5]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        if (!mouse_down)
        {
            //https://www.youtube.com/watch?v=fcomwmZAIpI

            // todo: do the test here
            if (mouse_pos.x > test_input.x && mouse_pos.x < test_input.w && mouse_pos.y > test_input.y && mouse_pos.y < test_input.h)
            {
                cLink new_link({links[0].data[0], links[0].data[1]}, {links[0].data[0]+20, links[0].data[1]}, {mouse_pos.x-20, mouse_pos.y}, {mouse_pos.x, mouse_pos.y});
                Push(new_link);
            }


            is_dragging_new_link = false;  // oh fuck...now I have to drop it on a valid input
        }
    }
   
    
    for(int i = 1; i<links.size(); i++)
    {
        if (!is_dragging_new_link)
        {
            if (!links[i].is_dragging)
            {
                if (mouse_down) links[i].is_dragging = true;

                links[i].TestHoverHandles(mouse_pos);
                if (links[i].handle_id > 0) {
                    UpdateSegmentColor(links[i].slot_id, links[i].handle_id);
                    links[i].is_hover = true;
                }
                else if (links[i].is_hover)
                {
                    ResetLinkColor(links[i].slot_id);
                    links[i].is_hover = false;
                }

            } else {
                if (!mouse_down) links[i].is_dragging = false;

                UpdateCtrlPtPos(links[i].slot_id, links[i].handle_id, mouse_pos);
            }
        }

        glDrawArrays(GL_LINE_STRIP, links[i].slot_id * 6, 6);
    }

    glBindVertexArray(0);
}
