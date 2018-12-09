#pragma once

#include "common.h"

#include <GLFW/glfw3.h>
#include <linmath/linmath.h>

#include <experimental/optional>
#include <vector>
#include <iostream>

namespace exp = std::experimental;

class RenderObject {
    
public:
    ~RenderObject() {
    
        glDeleteProgram(this->program);
        
        glDeleteBuffers(1, &this->vbo_cube_vertices);
        glDeleteBuffers(1, &this->vbo_cube_colors);
        glDeleteBuffers(1, &this->ibo_cube_elements);
        
        this->vertices.clear();
        this->colors.clear();
        this->elements.clear();
    }
    
public:
    
    static bool has_vertex_shader, has_fragment_shader;
    static GLuint vertex_shader, fragment_shader;
    static GLuint get_vertex_shader() {
        
        if (!has_vertex_shader) {
            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
            glCompileShader(vertex_shader);
            has_vertex_shader = true;
        }
        
        return vertex_shader;
    }
    
    static GLuint get_fragment_shader() {
        
        if (!has_fragment_shader) {
            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
            glCompileShader(fragment_shader);
            has_fragment_shader = true;
        }
        
        return fragment_shader;
    }
    
    void setup_shaders() {
        
        this->program = glCreateProgram();
        glAttachShader(this->program, get_vertex_shader());
        glAttachShader(this->program, get_fragment_shader());
        glLinkProgram(this->program);
        
        this->mvp_location = glGetUniformLocation(this->program, "MVP");
        this->vpos_location = glGetAttribLocation(this->program, "vPos");
        this->vcol_location = glGetAttribLocation(this->program, "vCol");
    }
    
    void init() {
        
        glGenBuffers(1, &vbo_cube_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_cube_colors);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_t) * this->colors.size(), this->colors.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &ibo_cube_elements);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);

        setup_shaders();
    }
    
    virtual void render() {
        
        glUseProgram(this->program);
        
        glEnableVertexAttribArray(this->vpos_location);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
        glVertexAttribPointer(
                              this->vpos_location, // attribute
                              3,                 // number of elements per vertex, here (x,y,z)
                              GL_FLOAT, GL_FALSE, 0, 0);
        
        glEnableVertexAttribArray(this->vcol_location);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
        glVertexAttribPointer(
                              this->vcol_location, // attribute
                              3,                 // number of elements per vertex, here (R,G,B)
                              GL_FLOAT, GL_FALSE, 0, 0);
        
        /* Push each element in buffer_vertices to the vertex shader */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
        glDrawElements(GL_TRIANGLES, this->elements.size(), GL_UNSIGNED_SHORT, 0);
        
        glDisableVertexAttribArray(this->vpos_location);
        glDisableVertexAttribArray(this->vcol_location);
    }
    
    virtual void update(float dt, int w, int h) {
        
        float ratio = w / (float) h;
        
        mat4x4_perspective(this->projection_matrix, 45.0, ratio, 0.1, 100.0);
        mat4x4_identity(this->model_matrix);
        mat4x4_translate_in_place(this->model_matrix, this->x, this->y, this->z);
        //mat4x4_scale(this->model_matrix, this->model_matrix, 0.1);
        mat4x4_rotate(this->model_matrix, this->model_matrix, 0.5, 0.5, 0.5, this->rotation);
        mat4x4 mvp;
        mat4x4_mul(mvp, this->projection_matrix, this->model_matrix);
        
        glUseProgram(this->program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    }
    
    void set_roation(float rot) {
        this->rotation = rot;
    }
    
    void set_translation(exp::optional<float> x, exp::optional<float> y, exp::optional<float> z) {
        if (x) this->x = x.value_or(0.0);
        if (y) this->y = y.value_or(0.0);
        if (z) this->z = z.value_or(0.0);
    }
    
    void translate(exp::optional<float> x, exp::optional<float> y, exp::optional<float> z) {
        if (x) this->x += x.value_or(0.0);
        if (y) this->y += y.value_or(0.0);
        if (z) this->z += z.value_or(0.0);
    }
    
protected:
    float rotation = {};
    
    // TODO: Use vec3 or similar
    float x = {};
    float y = {};
    float z = {};
    
protected:
    GLuint program;
    
    GLint mvp_location, vpos_location, vcol_location;
    mat4x4 model_matrix, projection_matrix;
    
    GLuint vbo_cube_vertices, vbo_cube_colors;
    GLuint ibo_cube_elements;
    
    std::vector<vertex_t> vertices;
    std::vector<color_t> colors;
    std::vector<GLushort> elements;
};

bool RenderObject::has_vertex_shader = false;
bool RenderObject::has_fragment_shader = false;
GLuint RenderObject::vertex_shader;
GLuint RenderObject::fragment_shader;
