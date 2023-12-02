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

    // window creation
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
       glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); }
    );


    // opengl init
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW error" << std::endl;
        return 1;
    }
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    // model loading
    SE::render_context ship_context = SE::s_mesh_manager.load("../models/spaceship.obj");
    SE::render_context sphere_context = SE::s_mesh_manager.load("../models/sphere.obj");

    // texture loading
    SE::texture_info earth_texture = SE::s_texture_manager.load_texture("../textures/earth.png", "earth_tex");
    SE::texture_info clouds_texture = SE::s_texture_manager.load_texture("../textures/clouds.jpg", "clouds_tex");
    SE::texture_info moon_texture = SE::s_texture_manager.load_texture("../textures/moon.jpg", "moon_tex");
    SE::texture_info ship_texture = SE::s_texture_manager.load_texture("../textures/spaceship.jpg", "ship_tex");
    SE::texture_info rust_texture = SE::s_texture_manager.load_texture("../textures/rust.jpg", "rust_tex");
    SE::texture_info scratches_texture = SE::s_texture_manager.load_texture("../textures/scratches.jpg", "scratches_tex");

    // shader creation
    GLuint default_program, earth_program, moon_program, ship_program;

    GLuint default_vert = SE::s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/default.vert");
    GLuint default_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/default.frag");
    GLuint earth_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/earth.frag");
    GLuint moon_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/moon.frag");
    GLuint ship_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/ship.frag");
    
    try {
        default_program = SE::s_shader_manager.create_program({default_vert, default_frag});
        earth_program = SE::s_shader_manager.create_program({default_vert, earth_frag});
        moon_program = SE::s_shader_manager.create_program({default_vert, moon_frag});
        ship_program = SE::s_shader_manager.create_program({default_vert, ship_frag});
    }
    catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    // populating scene
    SE::rigid_body sun(sphere_context, default_program, glm::vec3(0), glm::vec3(0), {});
    SE::rigid_body earth(sphere_context, earth_program, glm::vec3(0), glm::vec3(0), { earth_texture, clouds_texture });
    SE::rigid_body moon(sphere_context, moon_program, glm::vec3(0), glm::vec3(0), { moon_texture });

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


    SE::ship player(ship_context, ship_program, glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), { ship_texture, rust_texture, scratches_texture }, 0.05, 0.05);

    // camera setup
    SE::camera camera1(0.01, 200, {0, 0, 1}, {0, 0, 1});
    SE::s_renderer.set_active_camera(camera1);

 
    std::vector<SE::rigid_body*> bodies = { &sun, &player, &earth, &moon };
    SE::light_source sun_light = { {1, 0, 0}, "light_dir", {1, 1, 1}, "light_color"};

    float timeElapsed;
    while (!glfwWindowShouldClose(window)) {
        timeElapsed = static_cast<float>(glfwGetTime());
        glfwGetWindowSize(window, &width, &height);
        camera1.set_aspect_ratio(static_cast<double>(width) / height);
        SE::s_io_processor.process_input(window, camera1, player);
        SE::s_renderer.render(bodies, { sun_light }, timeElapsed);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}