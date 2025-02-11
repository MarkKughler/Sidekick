#include "../lib/glad/glad_4_6.h"
#include "../nublog.h"
#include "grid_2D.h"


gui::cGrid_2D::cGrid_2D(sPoint pt)
{
    float data[24] = {
        0.0f, 24.0f, 0.0f, 0.0f,
        0.0f,  pt.y, 0.0f, 1.0f,
        pt.x, 24.0f, 1.0f, 0.0f,
        pt.x, 24.0f, 1.0f, 0.0f,
        0.0f,  pt.y, 0.0f, 1.0f,
        pt.x,  pt.y, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, &data[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(float) * 4, (void*)0);
    
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void gui::cGrid_2D::Render()
{
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
