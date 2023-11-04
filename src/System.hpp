#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "Body.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <memory>

class System {
    private:
        Body central;
        glm::vec3 displacement;
        float circTime;
        std::vector<System> dependants;
    public:
        System(const Body b, const glm::vec3 d, const float t) : central(b), displacement(d), circTime(t) {}
        ~System() {}
        void attachSystem(System& dep) { dependants.push_back(dep); }
        std::vector<Body> calculatePositions(float time);
};

#endif