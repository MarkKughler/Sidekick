#include "../lib/glad/glad_4_6.h"
#include "../glsl/shader_line.h"
#include "line.h"

core::cLine::cLine()
{
    data.stride = 2;
    _vao = 0;
    _vbo = 0;        // todo: multiple buffers (ie: pos, color)
    //_cbo = 0; ???
}

core::cLine::~cLine()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);
}


bool core::cLine::Upload()
{
    if (data.vdata.size() == 0) return false;
   
    data.num_verts = data.vdata.size() / 2;
    GLsizei    vert_stride_size = data.stride * sizeof(float);
    GLsizeiptr vert_buffer_size = data.num_verts * vert_stride_size;

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_buffer_size, &data.vdata[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, vert_stride_size, (void*)0); // format: { posX, posY }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return true; // todo: error check???
}


void core::cLine::Render(int startVertex, int numSegments)
{
    int count = 0;
    if (numSegments == 0 || numSegments >= data.num_verts - startVertex)
        count = data.num_verts - startVertex;
    else count = numSegments + 1; // two vertices make a line segment
    
    glBindVertexArray(_vao);
    glDrawArrays(GL_LINE_STRIP, startVertex, count);
}


void core::cLine::PopVertex()
{
    data.vdata.pop_back();
    data.vdata.pop_back();
    data.num_verts--;
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, data.vdata.size() * sizeof(float), &data.vdata[0], GL_DYNAMIC_DRAW);
}


void core::cLine::PushVertex(float x, float y)
{
    data.vdata.push_back(x);
    data.vdata.push_back(y);
    data.num_verts++;
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, data.vdata.size() * sizeof(float), &data.vdata[0], GL_DYNAMIC_DRAW);
}
