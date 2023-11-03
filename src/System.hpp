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
        System(const Body& b, const glm::vec3& d, const float t) : central(b), displacement(d), circTime(t) {}
        ~System() {}
        void render(const glm::mat4& parentMatrix);
        glm::mat4 position(float time);
        void attachSystem(System& dep) { dependants.push_back(dep); }
};

#endif