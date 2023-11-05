#include "ModelLoader.hpp"
#include "ShaderLoader.hpp"
#include "Body.hpp"
#include "System.hpp"
#include "Camera.hpp"
#include "IOProcessor.hpp"
#include "Renderer.hpp"
#include "Ship.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <stdexcept>

#include "Sol.hpp"


GLuint program;
unsigned width = 500, height = 500;
Camera camera(0.05f, 200.f, {1, 0, 0}, {-4, 0, 0});


int main() {

    float timeElapsed;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width = height = 500;

    GLFWwindow* window = glfwCreateWindow(width, height, "FirstWindow", NULL, NULL);
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

    renderContext shipContext = ModelLoader::load("../models/spaceship.obj");
    Ship ship(shipContext, {1., 1., 1.}, 0.2);

    System SolSystem = createSol();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window)) {
        timeElapsed = static_cast<float>(glfwGetTime());
        std::vector<Body> bodiesToRender = SolSystem.calculatePositions(timeElapsed);
        IOProcessor::processInput(window, camera, ship);
        Renderer::render(program, bodiesToRender, ship);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ShaderLoader::deleteProgram(program);
    glfwTerminate();
}