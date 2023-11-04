#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include <vector>
#include <GL/glew.h>
#include "Body.hpp"

class Renderer {
    public:
        static void renderBodies(GLuint program, std::vector<Body>& bodies);
};

#endif