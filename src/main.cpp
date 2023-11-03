#include "ModelLoader.hpp"
#include "ShaderLoader.hpp"
#include "Body.hpp"
#include "System.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <stdexcept>

float timeElapsed;
GLuint program;

void processInput(GLFWwindow* window);

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "FirstWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW error" << std::endl;
        return 1;
    }
    
    try {
        program = ShaderLoader::createProgram("../shaders/shader.vert", "../shaders/shader.frag");
    }
    catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    renderContext sphereContext = ModelLoader::load("../models/sphere.obj");

    Body Sun{sphereContext, glm::vec3(0.9, 0.9, 0.2), 1.f};
    Body Earth{sphereContext, glm::vec3(0., 0.1, 0.9), 0.5f};
    Body Moon{sphereContext, glm::vec3(0.7, 0.7, 0.7), 0.2f};
    Body Venus{sphereContext, glm::vec3(0.5, 0.5, 0.), 0.5};

    std::vector<System> empty;

    System MoonSystem(Moon, { 0., 3.f, 0. }, 10.f, empty);
    System EarthSystem(Earth, { 0., 0., 3.5 }, 0.5f, { MoonSystem });
    System VenusSystem(Venus, { 0., 0., -3.5}, 0.5, empty);
    System SolSystem(Sun, glm::vec3(0.f), 0.f, { EarthSystem, VenusSystem });

    glViewport(0, 0, 500, 500);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        timeElapsed = static_cast<float>(glfwGetTime());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        SolSystem.render(glm::mat4(1.0));
        glUseProgram(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ShaderLoader::deleteProgram(program);
    glfwTerminate();
}