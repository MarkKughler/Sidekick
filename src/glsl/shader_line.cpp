
#include "../nublog.h"
#include "shader_line.h"
#include "../lib/glad/glad_4_6.h"
constexpr auto S_OK = 1;

glsl::cShader_line::cShader_line()
{
    _vs_id   = 0;
    _fs_id   = 0;
     prog_id = 0;

     loc_projection = 0;
     loc_color      = 0;
}


glsl::cShader_line::~cShader_line()
{
    glDetachShader(prog_id, _vs_id);
    glDetachShader(prog_id, _fs_id);
    glDeleteShader(_vs_id);
    glDeleteShader(_fs_id);
    glDeleteProgram(prog_id);
}


bool glsl::cShader_line::Create()
{
    const char* vsBuffer = R"(
        #version 400
        in vec2 pos;
        uniform mat4 projection;
        uniform vec2 offset;
        void main() 
        {
            vec2 p = pos + offset;
            gl_Position = vec4(p, 0.0, 1.0) * projection;
        })";
    
    const char* fsBuffer = R"(
        #version 400
        out vec4 out_color;
        uniform vec3 color;
        void main()
        {
            out_color = vec4(color, 1.0);
        })";

    _vs_id = glCreateShader(GL_VERTEX_SHADER);
    _fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_vs_id, 1, &vsBuffer, 0);
    glShaderSource(_fs_id, 1, &fsBuffer, 0);
    glCompileShader(_vs_id);
    glCompileShader(_fs_id);

    int status = S_OK;
    glGetShaderiv(_vs_id, GL_COMPILE_STATUS, &status);
    if (status != S_OK)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_line::Create", "Failed to compile line vertex shader");
        glGetShaderInfoLog(_vs_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log);
        return false;
    }
    glGetShaderiv(_fs_id, GL_COMPILE_STATUS, &status);
    if (status != S_OK)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_line::Create", "Failed to compile line fragment shader");
        glGetShaderInfoLog(_fs_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log);
        return false;
    }

    prog_id = glCreateProgram();
    glAttachShader(prog_id, _vs_id);
    glAttachShader(prog_id, _fs_id);
    glBindAttribLocation(prog_id, 0, "pos");
    glLinkProgram(prog_id);
    glGetProgramiv(prog_id, GL_LINK_STATUS, &status);
    if (status != S_OK)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_line::Create", "Failed to link line shader program");
        glGetProgramInfoLog(prog_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log);
        return false;
    }

    loc_projection = glGetUniformLocation(prog_id, "projection");
    loc_offset = glGetUniformLocation(prog_id, "offset");
    loc_color = glGetUniformLocation(prog_id, "color");
    LOG_INFO("cShader_line::Create", "shader program link success");
    return true;
}
