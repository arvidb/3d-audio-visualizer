#pragma once

#include "common.h"

#include <GLFW/glfw3.h>
#include <linmath/linmath.h>

#include <vector>

class RenderObject {
    
public:
    void setup_shaders() {
        
        GLuint vertex_shader, fragment_shader;
        
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);
        
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);
        
        this->program = glCreateProgram();
        glAttachShader(this->program, vertex_shader);
        glAttachShader(this->program, fragment_shader);
        glLinkProgram(this->program);
        
        this->mvp_location = glGetUniformLocation(this->program, "MVP");
        this->vpos_location = glGetAttribLocation(this->program, "vPos");
        this->vcol_location = glGetAttribLocation(this->program, "vCol");
        
        glEnableVertexAttribArray(this->vpos_location);
        glVertexAttribPointer(this->vpos_location, 2, GL_FLOAT, GL_FALSE,
                              sizeof(float) * 5, (void*) 0);
        
        glEnableVertexAttribArray(this->vcol_location);
        glVertexAttribPointer(this->vcol_location, 3, GL_FLOAT, GL_FALSE,
                              sizeof(float) * 5, (void*) (sizeof(float) * 2));
    }
    
    void init() {
        
        GLuint vertex_buffer;
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex_t) * vertices.size(),
                     vertices.data(),
                     GL_STATIC_DRAW);
        
        setup_shaders();
    }
    
    virtual void render() = 0;
    virtual void update(float dt, int w, int h) {
        
        float ratio = w / (float) h;
        mat4x4_ortho(this->projection_matrix, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        
        mat4x4_identity(this->model_matrix);
        mat4x4_translate_in_place(this->model_matrix, this->x, this->y, 0.0);
        mat4x4_rotate_Z(this->model_matrix, this->model_matrix, this->rotation);
        
        mat4x4 mvp;
        mat4x4_mul(mvp, this->projection_matrix, this->model_matrix);
        
        glUseProgram(this->program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    }
    
    void set_roation(float rot) {
        this->rotation = rot;
    }
    
    void set_translation(float x, float y) {
        this->x = x;
        this->y = y;
    }
    
protected:
    float rotation = {};
    float x = {};
    float y = {};
    
protected:
    GLuint program;
    GLint mvp_location, vpos_location, vcol_location;
    mat4x4 model_matrix, projection_matrix;
    std::vector<vertex_t> vertices;
};
