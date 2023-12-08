#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "mesh_manager.hpp"
#include "shader_manager.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "io_processor.hpp"
#include "object.hpp"
#include "ship.hpp"
#include "texture_manager.hpp"
#include "scene.hpp"
#include "light_source.hpp"

int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 1000;
float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); 
	WINDOW_WIDTH = width; 
	WINDOW_HEIGHT = height; 
	ASPECT_RATIO = (float)width / (float)height;
}

int main() {

    // window creation
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SpaceEngine", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // opengl init
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW error" << std::endl;
        return 1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // model loading
    SE::mesh_info ship_mesh = SE::s_mesh_manager.load("../models/spaceship.obj");
    SE::mesh_info sphere_mesh = SE::s_mesh_manager.load("../models/sphere.obj");
    SE::mesh_info cube_mesh = SE::s_mesh_manager.load("../models/cube.obj");

    // texture loading
    SE::texture_info earth_texture = SE::s_texture_manager.load_texture("../textures/earth.2png", "earth_tex");
    SE::texture_info clouds_texture = SE::s_texture_manager.load_texture("../textures/clouds.jpg", "clouds_tex");
    SE::texture_info moon_texture = SE::s_texture_manager.load_texture("../textures/moon.jpg", "moon_tex");
    SE::texture_info ship_texture = SE::s_texture_manager.load_texture("../textures/spaceship.jpg", "ship_tex");
    SE::texture_info rust_texture = SE::s_texture_manager.load_texture("../textures/rust.jpg", "rust_tex");
    SE::texture_info scratches_texture = SE::s_texture_manager.load_texture("../textures/scratches.jpg", "scratches_tex");

    // normal maps
    SE::texture_info earth_normals = SE::s_texture_manager.load_texture("../textures/earth2_normals.png", "earth_normals");
    SE::texture_info moon_normals = SE::s_texture_manager.load_texture("../textures/moon_normal.jpg", "moon_normals");
    SE::texture_info ship_normals = SE::s_texture_manager.load_texture("../texttures/spaceship_normal.jpg", "ship_normals");
    SE::texture_info rust_normals = SE::s_texture_manager.load_texture("../textures/rust_normal.jpg", "rust_normals");

    // cubemaps
    const char* walls[6] = {
        "../textures/skybox/space_rt.png", 
        "../textures/skybox/space_lf.png", 
        "../textures/skybox/space_up.png", 
        "../textures/skybox/space_dn.png", 
        "../textures/skybox/space_bk.png", 
        "../textures/skybox/space_ft_galaxy.png"
    };
    SE::texture_info skybox_cubemap = SE::s_texture_manager.load_cubemap(walls, "skybox");

    // shader creation
    GLuint default_program, earth_program, moon_program, ship_program, sun_program, skybox_program;

    GLuint default_vert = SE::s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/default.vert");
    GLuint default_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/default.frag");
    GLuint earth_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/earth.frag");
    GLuint moon_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/moon.frag");
    GLuint ship_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/ship.frag");
    GLuint sun_vert = SE::s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/sun.vert");
    GLuint sun_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/sun.frag");
    GLuint skybox_vert = SE::s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/skybox.vert");
    GLuint skybox_frag = SE::s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/skybox.frag");
    
    try {
        default_program = SE::s_shader_manager.create_program({default_vert, default_frag});
        earth_program = SE::s_shader_manager.create_program({default_vert, earth_frag});
        moon_program = SE::s_shader_manager.create_program({default_vert, moon_frag});
        ship_program = SE::s_shader_manager.create_program({default_vert, ship_frag});
        sun_program = SE::s_shader_manager.create_program({sun_vert, sun_frag});
        skybox_program = SE::s_shader_manager.create_program({skybox_vert, skybox_frag});
    }
    catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    // populating scene
	SE::scene solar_system;

	SE::skybox_info skybox = { skybox_cubemap, skybox_program, cube_mesh };

	solar_system.set_skybox(skybox);

	SE::object sun(sphere_mesh, sun_program, {});
	SE::object earth(sphere_mesh, earth_program, { earth_texture, clouds_texture, earth_normals });
	SE::object moon(sphere_mesh, moon_program, { moon_texture, moon_normals });

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

	std::vector<SE::texture_info> ship_textures = { ship_texture, rust_texture, scratches_texture, ship_normals, rust_normals };
	

	SE::ship player(ship_mesh, ship_program, ship_textures, glm::vec3(-5, 0, 0), glm::vec3(0, 0, 1), 0.05, 0.05);


	solar_system.set_objects({ &sun, &earth, &moon, &player});

	SE::light_source sunlight( {0, 0, 0}, "light_pos");

	solar_system.set_light_sources({sunlight});

    // camera setup
    SE::camera ship_camera(0.01, 2000, {1, 0, 0}, {-6, 0, 0});

	player.attach_camera(ship_camera);
    solar_system.set_camera(ship_camera);

    while (!glfwWindowShouldClose(window)) {
		SE::s_io_processor.process_input(window, player);
		SE::s_renderer.render(window, solar_system);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}