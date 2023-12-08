#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene.hpp"

namespace SE {

	class renderer {
		public:
			void render(const GLFWwindow* window, const scene& t_scene); // time is needed for object positions			
	};

	static renderer s_renderer;
}


#endif