#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "Body.hpp"

class Ship : public Body {
    friend class IOProcessor;
    friend class Renderer;
    private:
        glm::vec3 shipPos, shipDir;
    public:
        Ship(const RenderContext ctx, const glm::vec3 clr, float s, glm::vec3 initPos = {0., 0., 0.}, glm::vec3 initDir = {0., 0., 0}) : 
            Body(ctx, clr, s), shipPos(initPos), shipDir(initDir) {}
};

#endif