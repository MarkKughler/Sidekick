
#include <iostream>
#include "shader_gui.h"
#include "../lib/glad/glad_4_6.h"
constexpr auto S_OK = 1;

core::cShader_gui::cShader_gui()
{
    _vs_id = 0;
    _fs_id = 0;
    _prog_id = 0;
}


bool core::cShader_gui::Create()
{
    const char* vsBuffer = R"(
        #version 400
        in vec3 in_pos;
        in vec3 in_color;
        out vec3 color;
        uniform mat4 world;     
        void main()  
        {   
            gl_Position = vec4(in_pos, 1.0) * world;
            color = in_color;
        };)";

    const char* fsBuffer = R"(
        #version 400
        in vec3 color;
        out vec4 out_color;
        void main() 
        {
            out_color = vec4(color, 1.0);
        };)";
    
    _vs_id = glCreateShader(GL_VERTEX_SHADER);
    _fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_vs_id, 1, &vsBuffer, 0);
    glShaderSource(_fs_id, 1, &fsBuffer, 0);
    glCompileShader(_vs_id);
    glCompileShader(_fs_id);
    
    int status = S_OK;
    glGetShaderiv(_vs_id, GL_COMPILE_STATUS, &status);
    if (status != 1) 
    { 
        char info_log[1024] = "";
        std::cout << "Failed to compile gui vertex shader\n"; 
        glGetShaderInfoLog(_vs_id, 1024, 0, info_log);
        std::cout << info_log << std::endl;
        return false; 
    }
    glGetShaderiv(_fs_id, GL_COMPILE_STATUS, &status);
    if (status != 1) 
    { 
        char info_log[1024] = "";
        std::cout << "Failed to compile gui fragment shader\n"; 
        glGetShaderInfoLog(_fs_id, 1024, 0, info_log);
        std::cout << info_log << std::endl;
        return false; 
    }
    
    _prog_id = glCreateProgram();
    glAttachShader(_prog_id, _vs_id);
    glAttachShader(_prog_id, _fs_id);
    glBindAttribLocation(_prog_id, 0, "in_pos");
    glBindAttribLocation(_prog_id, 1, "in_color");
    glLinkProgram(_prog_id);
    glGetProgramiv(_prog_id, GL_LINK_STATUS, &status);
    if (status != 1) 
    { 
        char info_log[1024] = "";
        std::cout << "Failed to link gui shader\n"; 
        glGetProgramInfoLog(_prog_id, 1024, 0, info_log);
        std::cout << info_log;
        return false; 
    }

    return true;
}


void core::cShader_gui::Destroy()
{
    glDetachShader(_prog_id, _vs_id);
    glDetachShader(_prog_id, _fs_id);
    glDeleteShader(_vs_id);
    glDeleteShader(_fs_id);
    glDeleteProgram(_prog_id);
}


bool core::cShader_gui::SetParameters(float world[16])
{
    glUseProgram(_prog_id);
    int location = glGetUniformLocation(_prog_id, "world");
    glUniformMatrix4fv(location, 1, false, world);

    return true;
}

