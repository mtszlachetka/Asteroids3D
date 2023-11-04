#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include <vector>
#include <GL/glew.h>
#include "Body.hpp"
#include "Ship.hpp"

class Renderer {
    public:
        static void render(GLuint program, std::vector<Body>& bodies, Ship& ship);
};

#endif