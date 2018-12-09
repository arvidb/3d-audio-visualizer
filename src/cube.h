#pragma once

#include "render_object.h"

struct Cube : public RenderObject {
    
    Cube() {
        
        this->vertices = {
            // front
            {-1.0, -1.0,  1.0},
            {1.0, -1.0,  1.0},
            {1.0,  1.0,  1.0},
            {-1.0,  1.0,  1.0},
            // back
            {-1.0, -1.0, -1.0},
            {1.0, -1.0, -1.0},
            {1.0,  1.0, -1.0},
            {-1.0,  1.0, -1.0}
        };
        
        this->colors = {
            // front colors
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0},
            // back colors
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0}
        };
        
        this->elements = {
            // front
            0, 1, 2,
            2, 3, 0,
            // top
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // bottom
            4, 0, 3,
            3, 7, 4,
            // left
            4, 5, 1,
            1, 0, 4,
            // right
            3, 2, 6,
            6, 7, 3,
        };


        init();
    }
    
    void update(float dt, int w, int h) override {
        
        set_roation(dt);
        RenderObject::update(dt, w, h);
    }
};
