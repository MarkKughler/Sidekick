
#include "../nublog.h"
#include "shader_gui.h"
#include "../lib/glad/glad_4_6.h"
constexpr auto S_OK = 1;

glsl::cShader_gui::cShader_gui()
{
    _vs_id   = 0;
    _fs_id   = 0;
     prog_id = 0;

     loc_projection  = 0;
     loc_translation = 0;
     loc_offset      = 0;
     loc_color       = 0;
}


bool glsl::cShader_gui::Create()
{
    const char* vsBuffer = R"(
        #version 400
        in vec4 in_pos;
        out vec2 uv;
        uniform mat4 projection;
        uniform vec2 translation;    
        uniform vec2 offset; 
        void main()  
        {   
            vec2 p = in_pos.xy + translation + offset;
            gl_Position = vec4(p.xy, 0.0, 1.0) * projection;
            uv = in_pos.zw;
        };)";

    const char* fsBuffer = R"(
        #version 400
        in vec2 uv;
        out vec4 out_color;
        uniform sampler2D text;
        uniform vec3 color;
        void main() 
        {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, uv).r);
            out_color = vec4(color, 1.0) * sampled;
        };)";

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
        LOG_ERROR("cShader_gui::Create", "Failed to compile gui vertex shader")
        glGetShaderInfoLog(_vs_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log)
        return false;
    }
    glGetShaderiv(_fs_id, GL_COMPILE_STATUS, &status);
    if (status != S_OK)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_gui::Create", "Failed to compile gui fragment shader")
        glGetShaderInfoLog(_fs_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log)
        return false;
    }

    prog_id = glCreateProgram();
    glAttachShader(prog_id, _vs_id);
    glAttachShader(prog_id, _fs_id);
    glBindAttribLocation(prog_id, 0, "in_pos");
    glBindAttribLocation(prog_id, 1, "in_color");
    glLinkProgram(prog_id);
    glGetProgramiv(prog_id, GL_LINK_STATUS, &status);
    if (status != S_OK)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_gui::Create", "Failed to link gui shader program")
        glGetProgramInfoLog(prog_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log)
        return false;
    }

    loc_projection  = glGetUniformLocation(prog_id, "projection");
    loc_translation = glGetUniformLocation(prog_id, "translation");
    loc_offset      = glGetUniformLocation(prog_id, "offset");
    loc_color       = glGetUniformLocation(prog_id, "color");
    return true;
}


void glsl::cShader_gui::Destroy() const
{
    glDetachShader(prog_id, _vs_id);
    glDetachShader(prog_id, _fs_id);
    glDeleteShader(_vs_id);
    glDeleteShader(_fs_id);
    glDeleteProgram(prog_id);
}


bool glsl::cShader_gui::SetParameters(float world[16]) const
{
    // todo : all of this
    glUseProgram(prog_id);
    int location = glGetUniformLocation(prog_id, "world");
    glUniformMatrix4fv(location, 1, false, world);

    return true;
}

