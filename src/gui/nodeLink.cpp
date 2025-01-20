#include "../lib/glad/glad_4_6.h"
#include "nodeLink.h"


gui::cLink::cLink(sPoint start, sPoint ctrl_pt1, sPoint ctrl_pt2, sPoint end)
{
    sPoint start_tail = { start.x + 21, start.y };
    sPoint end_tail   = {   end.x - 21,   end.y };

    vdata[0] = start;
    vdata[1] = start_tail;
    vdata[2] = ctrl_pt1;
    vdata[3] = ctrl_pt2;
    vdata[4] = end_tail;
    vdata[5] = end;

    bounds = { 0 };
    // todo: prefer the cdata vertex color idea better. (revert back)
}


void gui::cLink::Push()
{

}


void gui::cLink::Pop()
{

}

/*std::vector<unsigned int> indices;
void gui::cLinkContainer::CalculateIndexData(int n)
{    
    for (int i = 0; i < n; i++)
    {
        int offset = 6 * i;
        indices.push_back(offset + 0); indices.push_back(offset + 1);
        indices.push_back(offset + 1); indices.push_back(offset + 2);
        indices.push_back(offset + 2); indices.push_back(offset + 3);
        indices.push_back(offset + 3); indices.push_back(offset + 4);
        indices.push_back(offset + 4); indices.push_back(offset + 5);
    }
}*/


void gui::cLinkContainer::Initialize()
{
    int vbuffer_size = sizeof(float) * 6 * (2/*(x,y)*/);// *3/*rgb*/);
    //int ibuffer_size = sizeof(unsigned int) * 10/*index elements*/;
    
    int N = 10;// pre allocate a larger space for (N) links
    vbuffer_size *= N; 
    //ibuffer_size *= N;
    //CalculateIndexData(N);

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    //glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vbuffer_size, nullptr, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float)*2, (void*)0);                    // x,y
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float)*3, (void*)(sizeof(float) * 12)); // rgb
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibuffer_size, &indices[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
}


void gui::cLinkContainer::Add(cLink item)
{
    links.push_back(item);
    
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 12, &item.vdata[0]);
   // glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 12, sizeof(float) * 18, &item.cdata[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void gui::cLinkContainer::Render(unsigned int color_loc)
{
    sRGB white = { 0.9f, 0.9f, 0.9f };
    sRGB hover = { 0.0f, 0.5f, 1.0f };
    glBindVertexArray(_vao);
    //glDrawElements(GL_LINES, 10, GL_UNSIGNED_INT, &indices[0]);
    glDrawArrays(GL_LINE_STRIP, 0, 3);
    glUniform3f(color_loc, hover.r, hover.g, hover.b);
    glDrawArrays(GL_LINE_STRIP, 2, 2);
    glUniform3f(color_loc, white.r, white.g, white.b);
    glDrawArrays(GL_LINE_STRIP, 3, 3);
}
