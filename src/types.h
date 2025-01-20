#pragma once
#include <vector>

constexpr float pi    = 3.1415926535897932384626433832795f;
constexpr float twopi = 6.283185307179586476925286766559f;
constexpr float hlfpi = 1.5707963267948966192313216916398f;
static float degtorad(float deg) { return deg * 0.01745329251994329576923690768489f; }
static float radtodeg(float rad) { return rad / 0.01745329251994329576923690768489f; }

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

struct sRect
{
    int x;
    int y;
    int w;
    int h;
};

struct sVec2
{
    float x;
    float y;

    inline float dot(const sVec2& v1, const sVec2& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline void normalize()
    {
        float ln = sqrtf((x * x) + (y * y));
        x *= ln;
        y *= ln;
    }

};

struct sModelFormat
{
    size_t num_verts;
    size_t num_indices;
    int stride;
    std::vector<float> vdata;
    std::vector<unsigned int> idata;
};

struct sRGB
{
    float r;
    float g;
    float b;
};

struct sLineFormat
{
    size_t num_verts;
    int stride;
    std::vector<sPoint> vdata;
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
