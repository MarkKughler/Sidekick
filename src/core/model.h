#pragma once

namespace core
{

    class cModel
    {

    public:

        cModel();
        cModel(cModel&) = delete;
        ~cModel() { /*empty destructor*/ }

        /*
        Parameter Info:
        number of vertices
        number of indices 
        number of float elements per vertex 
        vertex data
        index data 
        */
        bool Upload(int, int, int, float*, unsigned int*);
        void Destroy();
        void Render();

    private:

        int _indices_count;

        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

    };

}
