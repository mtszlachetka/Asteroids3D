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
#include "texture_manager.hpp"


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
    std::vector<SE::shader_info> default_shaders = {
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
        program = SE::s_shader_manager.create_program(default_shaders);
    }
    catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    SE::render_context ship_context = SE::s_mesh_manager.load("../models/spaceship.obj");
    SE::render_context sphere_context = SE::s_mesh_manager.load("../models/sphere.obj");

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    SE::texture_info earth_texture = SE::s_texture_manager.load_texture("../textures/earth.png", "earth_tex");
    std::vector<SE::texture_info> earth_textures = { earth_texture };

    SE::rigid_body sun(sphere_context, program, glm::vec3(0), glm::vec3(0), std::vector<SE::texture_info>());
    SE::rigid_body earth(sphere_context, program, glm::vec3(0), glm::vec3(0), std::vector<SE::texture_info>());
    SE::rigid_body moon(sphere_context, program, glm::vec3(0), glm::vec3(0), std::vector<SE::texture_info>());

    earth.set_position_callback(
        [](float time) -> glm::mat4 { return glm::rotate(glm::mat4(1.), time / 10, {0, 1, 0}) * 
                                                glm::translate(glm::mat4(1.), glm::vec3(4.f, 0, 0)) * 
                                                glm::scale(glm::mat4(1.),glm::vec3(0.3f));}
    );

    moon.set_position_callback(
        [](float time) -> glm::mat4 { return glm::rotate(glm::mat4(1.), time / 10, {0, 1, 0}) * 
                                                glm::translate(glm::mat4(1.), glm::vec3(4.f, 0, 0)) * 
                                                glm::rotate(glm::mat4(1.), time, {0, 1, 0}) * 
                                                glm::translate(glm::mat4(1.), glm::vec3(1.f, 0, 0)) * 
                                                glm::scale(glm::mat4(1.),glm::vec3(0.1f));}
    );

    SE::ship player(ship_context, program, glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), std::vector<SE::texture_info>(), 0.05, 0.05);

    SE::camera camera1(0.01, 200, {0, 0, 1}, {0, 0, 1});
    SE::s_renderer.set_active_camera(camera1);

    glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); }
    );

    std::vector<SE::rigid_body*> bodies = { &sun, &player, &earth, &moon };

    while (!glfwWindowShouldClose(window)) {
        timeElapsed = static_cast<float>(glfwGetTime());
        glfwGetWindowSize(window, &width, &height);
        camera1.set_aspect_ratio(static_cast<double>(width) / height);
        SE::s_io_processor.process_input(window, camera1, player);
        SE::s_renderer.render(bodies, timeElapsed);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    SE::s_shader_manager.delete_program(program);
    glfwTerminate();
}