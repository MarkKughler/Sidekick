#include "../lib/glad/glad_4_6.h"
#include "model.h"


core::cModel::cModel() 
{
    _indices_count = 0;
    _vao = 0;
    _vbo = 0;
    _ebo = 0;
}


bool core::cModel::Upload(int num_vertices, int num_indices, int num_vert_elements,
                              float* vert_data, unsigned int* index_data)
{
    _indices_count = num_indices;

    GLsizei    vert_stride_size  = num_vert_elements * sizeof(float);
    GLsizeiptr vert_buffer_size  = num_vertices * vert_stride_size;
    GLsizeiptr index_buffer_size = num_indices * sizeof(unsigned int);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_buffer_size, vert_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, vert_stride_size, 0);
    //glVertexAttribPointer(1, 2, GL_FLOAT, false, vert_stride_size, (unsigned char*)nullptr + (2 * sizeof(float)));
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, index_data, GL_STATIC_DRAW);

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


void core::cModel::Render() const
{
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices_count, GL_UNSIGNED_INT, 0);
}
