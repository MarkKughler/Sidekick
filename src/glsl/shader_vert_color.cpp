
#include "../nublog.h"
#include "shader_vert_color.h"
#include "../lib/glad/glad_4_6.h"
constexpr auto S_OK = 1;

glsl::cShader_vert_color::cShader_vert_color()
{
    _vs_id = 0;
    _fs_id = 0;
    prog_id = 0;

    loc_projection = 0;
    loc_offset = 0;
}


glsl::cShader_vert_color::~cShader_vert_color()
{
    glDetachShader(prog_id, _vs_id);
    glDetachShader(prog_id, _fs_id);
    glDeleteShader(_vs_id);
    glDeleteShader(_fs_id);
    glDeleteProgram(prog_id);
}


bool glsl::cShader_vert_color::Create()
{
    const char* vsBuffer = R"(
        #version 400
        in vec2 pos;
        in vec3 in_color;
        out vec3 out_color;
        uniform mat4 projection;
        uniform vec2 offset;
        void main() 
        {
            vec2 p = pos + offset;
            gl_Position = vec4(p, 0.0, 1.0) * projection;
            out_color = in_color;
        })";

    const char* fsBuffer = R"(
        #version 400
        in vec3 out_color;
        out vec4 pixel_color;
        void main()
        {
            pixel_color = vec4(out_color, 1.0);
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
        LOG_ERROR("cShader_vert_color::Create", "Failed to compile vertex shader");
        glGetShaderInfoLog(_vs_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log);
        return false;
    }
    glGetShaderiv(_fs_id, GL_COMPILE_STATUS, &status);
    if (status != S_OK)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_vert_color::Create", "Failed to compile fragment shader");
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
        LOG_ERROR("cShader_vert_color::Create", "Failed to link shader program");
        glGetProgramInfoLog(prog_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log);
        return false;
    }

    loc_projection = glGetUniformLocation(prog_id, "projection");
    loc_offset = glGetUniformLocation(prog_id, "offset");
    LOG_INFO("cShader_vert_color::Create", "shader program link success");
    return true;
}
