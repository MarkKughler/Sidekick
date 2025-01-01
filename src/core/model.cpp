#include "../lib/glad/glad_4_6.h"
#include "../glsl/shader_gui.h"
#include "model.h"


core::cModel::cModel() 
{
    _num_indices = 0;
    _prog_id = 0;
    _vao = 0;
    _vbo = 0;
    _ebo = 0;
}


bool core::cModel::Upload(unsigned int shader_id, sModelFormat format)
{
    _prog_id = shader_id;
    _num_indices = format.idata.size();

    GLsizei    vert_stride_size  = format.stride * sizeof(float);
    GLsizeiptr vert_buffer_size  = format.vdata.size() * vert_stride_size;
    GLsizeiptr index_buffer_size = format.idata.size() * sizeof(unsigned int);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_buffer_size, &format.vdata[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, vert_stride_size, 0); // format: { posX, posY, U, V }
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, &format.idata[0], GL_STATIC_DRAW);

    return true; // todo: error check???
}


void core::cModel::Destroy() const
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_ebo);
}


void core::cModel::Render(int x, int y, sColor color) const
{
    glUniform3f(glGetUniformLocation(_prog_id, "color"), color.r, color.g, color.b);
    glUniform2f(glGetUniformLocation(_prog_id, "offset"), x, y);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
    glUniform2f(glGetUniformLocation(_prog_id, "offset"), 0.0f, 0.0f); // reset position offset
}
