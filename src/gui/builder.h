#pragma once

namespace gui
{
    struct sBuilder
    {
        /*
        Parameter Info:
            model data destination
            desired width offset (default 24px)
            desired height offset (default 24px)
        */
        void NineSquare(sModelFormat &model, float w, float h)
        {
            model.num_verts = 36;
            model.num_indices = 54;
            model.stride = 4;

            model.vdata = {
                0.0f,      8.0f,     0.0f,      0.015625f, // top row of nine square
                8.0f,      8.0f,     0.015625f, 0.015625f,
                0.0f,      0.0f,     0.0f,      0.0f,
                8.0f,      0.0f,     0.015625f, 0.0f,
                8.0f,      8.0f,     0.015625f, 0.015625f, // top center
               16.0f + w,  8.0f,     0.03125f,  0.015625f,
                8.0f,      0.0f,     0.015625f, 0.0f,
               16.0f + w,  0.0f,     0.03125f,  0.0f,
               16.0f + w,  8.0f,     0.03125f,  0.015625f, // top right
               24.0f + w,  8.0f,     0.046875f, 0.015625f,
               16.0f + w,  0.0f,     0.03125f,  0.0f,
               24.0f + w,  0.0f,     0.046875f, 0.0f,

                0.0f,     16.0f + h, 0.0f,      0.03125f,  // center row
                8.0f,     16.0f + h, 0.015625f, 0.03125f,
                0.0f,      8.0f,     0.0f,      0.015625f,
                8.0f,      8.0f,     0.015625f, 0.015625f,
                8.0f,     16.0f + h, 0.015625f, 0.03125f,  // center square
               16.0f + w, 16.0f + h, 0.03125f,  0.03125f,
                8.0f,      8.0f,     0.015625f, 0.015625f,
               16.0f + w,  8.0f,     0.03125f,  0.015625f,
               16.0f + w, 16.0f + h, 0.03125f,  0.03125f,  // center right
               24.0f + w, 16.0f + h, 0.046875f, 0.03125f,
               16.0f + w,  8.0f,     0.03125f,  0.015625f,
               24.0f + w,  8.0f,     0.046875f, 0.015625f,
               
                0.0f,     24.0f + h, 0.0f,      0.046875f, // bottom row
                8.0f,     24.0f + h, 0.015625f, 0.046875f,
                0.0f,     16.0f + h, 0.0f,      0.03125f,
                8.0f,     16.0f + h, 0.015625f, 0.03125f,
                8.0f,     24.0f + h, 0.015625f, 0.046875f, // bottom center
               16.0f + w, 24.0f + h, 0.03125f,  0.046875f,
                8.0f,     16.0f + h, 0.015625f, 0.03125f,
               16.0f + w, 16.0f + h, 0.03125f,  0.03125f,
               16.0f + w, 24.0f + h, 0.03125f,  0.046875f, // bottom right
               24.0f + w, 24.0f + h, 0.046875f, 0.046875f,
               16.0f + w, 16.0f + h, 0.03125f,  0.03125f,
               24.0f + w, 16.0f + h, 0.046875f, 0.03125f
            };

            model.idata = { 0,  1,  2,  2,  1,  3,   4,  5,  6,  6,  5,  7,   8,  9, 10, 10,  9, 11,
                           12, 13, 14, 14, 13, 15,  16, 17, 18, 18, 17, 19,  20, 21, 22, 22, 21, 23,
                           24, 25, 26, 26, 25, 27,  28, 29, 30, 30, 29, 31,  32, 33, 34, 34, 33, 35 };
        }

        /*
        Parameter Info:
            model data destination
            desired width offset (default 24px)
            desired height offset (default 24px)
        */
        void GradSquare(sModelFormat &model, float w, float h)
        {
            model.num_verts = 4;
            model.num_indices = 6;
            model.stride = 4;
            model.vdata = {
                 0.0f,     24.0f + h, 0.09765f, 0.046875f,
                24.0f + w, 24.0f + h, 0.14062f, 0.046875f,
                 0.0f,      0.0f,     0.09765f, 0.0f,
                24.0f + w,  0.0f,     0.14062f, 0.0f            
            };
            model.idata = { 0, 1, 2, 2, 1, 3 };
        }


        void Icon(sModelFormat &model, int col, int row)
        {
            float c1 = (col * 24) / 512.f;
            float c2 = c1 + (24 / 512.f);
            float r1 = ((row * 24) + 32) / 512.f; // first row offset (32 px)
            float r2 = r1 + (24.0f / 512.f);
            model.num_verts = 4;
            model.num_indices = 6;
            model.stride = 4;
            model.vdata = {
                 0.0f, 24.0f, c1, r2,
                24.0f, 24.0f, c2, r2,
                 0.0f,  0.0f, c1, r1,
                24.0f,  0.0f, c2, r1
            };
            model.idata = { 0, 1, 2, 2, 1, 3 };
        }

    };

}