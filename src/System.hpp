#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "Body.hpp"
#include <vector>
#include <glm/glm.hpp>

class System {
    private:
        Body central;
        glm::vec3 displacement;
        float circTime;
        std::vector<System> dependants;
    public:
        System(const Body& b, const glm::vec3& d, const float t, const std::vector<System>& dep) : central(b), displacement(d), circTime(t), dependants(dep) {}
        ~System() {}
        void render(const glm::mat4& parentMatrix);
};

#endif