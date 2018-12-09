#pragma once

static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

typedef struct vertex
{
    vertex(float x, float y, float z)
    : x(x), y(y), z(z)
    {}
    
    float x, y, z;
} vertex_t;

typedef struct color
{
    color(float r, float g, float b)
    : r(r), g(g), b(b)
    {}
    
    float r, g, b;
} color_t;
