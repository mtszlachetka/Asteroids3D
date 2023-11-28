#ifndef _IOPROCESSOR_HPP_
#define _IOPROCESSOR_HPP_

#include "ship.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>

namespace SE {
    class io_processor {
        private:
            double m_lastx, m_lasty, m_pitch, m_yaw;     
        public:
            void process_input(GLFWwindow* win, camera& cam, ship& sh);
    };

    static io_processor s_io_processor;
}



#endif