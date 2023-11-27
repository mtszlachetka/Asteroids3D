#ifndef _IOPROCESSOR_HPP_
#define _IOPROCESSOR_HPP_

#include "ship.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>

class io_processor {
    public:
        void process_input(GLFWwindow* win, camera& cam, ship& sh);
};

static io_processor s_io_processor;

#endif