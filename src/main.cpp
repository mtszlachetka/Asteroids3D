#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "mesh_manager.hpp"
#include "shader_manager.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "io_processor.hpp"
#include "ship.hpp"


int main() {

    float timeElapsed;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 500, height = 500;

    GLFWwindow* window = glfwCreateWindow(width, height, "FirstWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW error" << std::endl;
        return 1;
    }
    GLuint program;
    std::vector<shader_info> default_shaders = {
        {
            GL_VERTEX_SHADER,
            "../shaders/shader.vert",
            "vertex shader"
        },
        {
            GL_FRAGMENT_SHADER,
            "../shaders/shader.frag",
            "fragment shader"
        }
    };
    try {
        program = s_shader_manager.create_program(default_shaders);
    }
    catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    render_context ship_context = s_mesh_manager.load("../models/spaceship.obj");
    render_context sphere_context = s_mesh_manager.load("../models/sphere.obj");

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    rigid_body sun(sphere_context, program, glm::vec3(0., 0., 0.), glm::vec3(0., 0., 1.), glm::vec3(0.9, 0.9, 0.2), std::vector<texture_info>());
    ship player(ship_context, program, glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(1), std::vector<texture_info>(), 0.05, 0.05);

    camera camera1(0.01, 200, {0, 0, 1}, {0, 0, 1});
    s_renderer.set_active_camera(camera1);

    glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); }
    );

    std::vector<rigid_body*> bodies = { &sun, &player };

    while (!glfwWindowShouldClose(window)) {
        timeElapsed = static_cast<float>(glfwGetTime());
        glfwGetWindowSize(window, &width, &height);
        camera1.set_aspect_ratio(static_cast<double>(width) / height);
        s_io_processor.process_input(window, camera1, player);
        s_renderer.render(bodies);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    s_shader_manager.delete_program(program);
    glfwTerminate();
}