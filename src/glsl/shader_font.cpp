
#include "../nublog.h"
#include "shader_font.h"
#include "../lib/glad/glad_4_6.h"
constexpr auto S_OK = 1;

glsl::cShader_font::cShader_font()
{
    _vs_id = 0;
    _fs_id = 0;
     prog_id = 0;
}


bool glsl::cShader_font::Create()
{
    const char* vsBuffer = R"(
        #version 330
        layout (location = 0) in vec4 in_pos;
        out vec2 tex_coords;
        uniform mat4 projection;     
        void main()  
        {   
            gl_Position = vec4(in_pos.xy, 0.0, 1.0) * projection;
            tex_coords = in_pos.zw;
        };)";

    const char* fsBuffer = R"(
        #version 330
        in vec2 tex_coords;
        out vec4 out_color;
        uniform sampler2D text;
        uniform vec3 text_color;
        void main() 
        {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coords).r);
            out_color = vec4(text_color, 1.0) * sampled;
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
        LOG_ERROR("cShader_font::Create", "Failed to compile font vertex shader")
        glGetShaderInfoLog(_vs_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log)
        return false;
    }
    glGetShaderiv(_fs_id, GL_COMPILE_STATUS, &status);
    if (status != 1)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_font::Create", "Failed to compile font fragment shader")
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
    if (status != 1)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_font::Create", "Failed to link font shader program")
        glGetProgramInfoLog(prog_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log)
        return false;
    }

    return true;
}


void glsl::cShader_font::Destroy()
{
    glDetachShader(prog_id, _vs_id);
    glDetachShader(prog_id, _fs_id);
    glDeleteShader(_vs_id);
    glDeleteShader(_fs_id);
    glDeleteProgram(prog_id);
}


bool glsl::cShader_font::SetParameters(float world[16])
{
    // todo : all of this
    glUseProgram(prog_id);
    int location = glGetUniformLocation(prog_id, "world");
    glUniformMatrix4fv(location, 1, false, world);

    return true;
}
