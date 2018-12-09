#pragma once

#include "render_object.h"

struct Triangle : public RenderObject {
    
    Triangle() {
        
        this->vertices.emplace_back(-0.6f, -0.4f, 1.f, 0.f, 0.f);
        this->vertices.emplace_back( 0.6f, -0.4f, 0.f, 1.f, 0.f);
        this->vertices.emplace_back( 0.f,   0.6f, 0.f, 0.f, 1.f);
        
        init();
    }
    
    void render() override {
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

struct RotatingTriangle : public Triangle {
    
    void update(float dt, int w, int h) override {
        
        set_roation(dt);
        RenderObject::update(dt, w, h);
    }
};
