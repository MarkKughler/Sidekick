#pragma once
#include <vector>

struct sDims
{
    int x;
    int y;
};

struct sSubDim
{
    size_t start;
    size_t count;
};

struct sPoint
{
    float x;
    float y;
};

struct sModelFormat
{
    size_t num_verts;
    size_t num_indices;
    int stride;
    std::vector<float> vdata;
    std::vector<unsigned int> idata;
};

struct sLineFormat
{
    size_t num_verts;
    int stride;
    std::vector<float> vdata;
};

struct sRGB
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

struct sConfiguration
{
    bool is_maxamized;
    sDims screen;
    sDims monitor;
    sMatrix ortho;
};
