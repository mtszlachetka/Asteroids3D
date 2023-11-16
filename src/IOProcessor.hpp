#ifndef _IOPROCESSOR_HPP_
#define _IOPROCESSOR_HPP_
#include "Camera.hpp"
#include "Ship.hpp"
#include <GLFW/glfw3.h>

class IOProcessor {
    private:
        static constexpr float angleSpeed = 0.05, moveSpeed = 0.05;
    public:
        void processInput(GLFWwindow* win, Camera& cam, Ship& ship);
};

static IOProcessor gIOProcessor;

#endif