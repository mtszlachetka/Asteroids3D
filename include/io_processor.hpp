#ifndef _IOPROCESSOR_HPP_
#define _IOPROCESSOR_HPP_

#include "object.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>

namespace se {
    class io_processor {
        public:
            void process_input(GLFWwindow* win, controllable_object& sh);
    };

    static io_processor s_io_processor;
}



#endif