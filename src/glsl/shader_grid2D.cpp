
#include "../nublog.h"
#include "shader_grid2D.h"
#include "../lib/glad/glad_4_6.h"
constexpr auto S_OK = 1;

glsl::cShader_grid2D::cShader_grid2D()
{
    _vs_id = 0;
    _fs_id = 0;
    prog_id = 0;

    loc_projection = 0;
    //loc_color = 0;
    //loc_offset = 0;
}


glsl::cShader_grid2D::~cShader_grid2D()
{
    glDetachShader(prog_id, _vs_id);
    glDetachShader(prog_id, _fs_id);
    glDeleteShader(_vs_id);
    glDeleteShader(_fs_id);
    glDeleteProgram(prog_id);
}


bool glsl::cShader_grid2D::Create()
{
    const char* vsBuffer = R"(
        #version 400
        in vec4 vert;
        out vec2 uv;               // not used
        out vec2 pos;
        uniform mat4 projection;
        
        void main() 
        {
            pos = vert.xy;
            uv = vert.zw;
            gl_Position = vec4(vert.xy, 0.0, 1.0) * projection;
        })";

    const char* fsBuffer = R"(
        #version 400
        in vec2 pos;
        in vec2 uv;                // not used
        uniform vec2 screenRes;
        uniform vec2 offset;
        uniform float zoom;
        out vec4 out_color;

        float grid(vec2 fragCoord, float space, float gridWidth)
        {
            vec2 fc = fragCoord - vec2(0.5);
            vec2 size = vec2(gridWidth);
            vec2 a1 = mod(fc - size, space);
            vec2 a2 = mod(fc + size, space);
            vec2 a = a2 - a1;
            float g = min(a.x, a.y);
            return clamp(g, 0.0, 1.0);
        }

        void main()
        {
            vec3 color = vec3(0.1, 0.15, 0.2);
            vec2 center = screenRes.xy / 2.0;
            color *= (1.0 - length(center-pos) / screenRes.x * 1.22);
            color *= clamp(grid(pos-(offset*zoom), 48.0*zoom, 0.5) * grid(pos-(offset*zoom), 240.0*zoom, 1.0), 0.7, 1.0);
            
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
        LOG_ERROR("cShader_grid2D::Create", "Failed to compile grid2D vertex shader");
        glGetShaderInfoLog(_vs_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log);
        return false;
    }
    glGetShaderiv(_fs_id, GL_COMPILE_STATUS, &status);
    if (status != S_OK)
    {
        char info_log[1024] = "";
        LOG_ERROR("cShader_grid2D::Create", "Failed to compile grid2D fragment shader");
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
        LOG_ERROR("cShader_grid2D::Create", "Failed to link grid2D shader program");
        glGetProgramInfoLog(prog_id, 1024, 0, info_log);
        LOG_ERROR("Info Log", info_log);
        return false;
    }

    loc_projection = glGetUniformLocation(prog_id, "projection");
    loc_screenRes  = glGetUniformLocation(prog_id, "screenRes");
    loc_offset     = glGetUniformLocation(prog_id, "offset");
    loc_zoom       = glGetUniformLocation(prog_id, "zoom");
    LOG_INFO("cShader_grid2D::Create", "shader program link success");
    return true;
}
