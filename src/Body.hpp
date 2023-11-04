#ifndef _BODY_HPP_
#define _BODY_HPP_

#include "ModelLoader.hpp"
#include "glm/glm.hpp"

class Body {
    friend class Renderer;
    friend class System;
    private:
        glm::mat4 transformMatrix;
    public:
        float scale;
        renderContext context;
        glm::vec3 color;
        Body(const renderContext ctx, const glm::vec3 clr, float s) : context(ctx), color(clr), scale(s) {}
        ~Body() {}
};


#endif