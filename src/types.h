#pragma once
#include <vector>

struct sDims
{
    int x;
    int y;
};

struct sModelFormat
{
    // todo: rethink if num_verts, num_indices is needed
    int stride;
    std::vector<float> vdata;
    std::vector<unsigned int> idata;
};

struct sColor
{
    float r;
    float g;
    float b;
};

struct sMatrix
{
    float mtx[16];
    // todo: math operations
};