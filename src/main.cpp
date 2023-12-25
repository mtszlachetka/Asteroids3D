#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "mesh_manager.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "io_processor.hpp"
#include "texture_manager.hpp"
#include "scene.hpp"
#include "read_file.hpp"

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;
float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
float time_elapsed = 0.f;
float delta_time = 0.f;
float last_time = 0.f;

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
	glClearColor(0.3,0.3,0.3,1);

    // model loading
    se::mesh_info ship_mesh = se::s_mesh_manager.load("../models/spaceship.obj");
    se::mesh_info sphere_mesh = se::s_mesh_manager.load("../models/sphere.obj");
    se::mesh_info cube_mesh = se::s_mesh_manager.load("../models/cube.obj");


	// // textures for PBR
	se::texture_info ship_diff = se::s_texture_manager.load_texture("../textures/spaceshipPBR/diff.png", "diffuse_map");
	se::texture_info ship_normals = se::s_texture_manager.load_texture("../textures/spaceshipPBR/norm.png", "normal_map");
	se::texture_info ship_amr = se::s_texture_manager.load_texture("../textures/spaceshipPBR/amr.png", "amr_map");

	se::texture_info merc_diff = se::s_texture_manager.load_texture("../textures/rock/diff.jpg", "diffuse_map");
	se::texture_info merc_normals = se::s_texture_manager.load_texture("../textures/rock/norm.jpg", "normal_map");
	se::texture_info merc_amr = se::s_texture_manager.load_texture("../textures/rock/arm.jpg", "amr_map");

    // // cubemaps
    const char* walls[6] = {
        "../textures/skybox/space_rt.png", 
        "../textures/skybox/space_lf.png", 
        "../textures/skybox/space_up.png", 
        "../textures/skybox/space_dn.png", 
        "../textures/skybox/space_bk.png", 
        "../textures/skybox/space_ft_galaxy.png"
    };
    se::texture_info skybox_cubemap = se::s_texture_manager.load_cubemap(walls, "skybox");

    // // shader creation
    GLuint star_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/sun.vert"));
    GLuint star_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/sun.frag"));

	GLuint punct_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/punctual.vert"));
	GLuint punct_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/punctual.frag"));
	
    GLuint skybox_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/skybox.vert"));
    GLuint skybox_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/skybox.frag"));
	
	GLuint star_program, skybox_program, punct_program;
	try {
		star_program = se::make_program({star_vert, star_frag});
		skybox_program = se::make_program({skybox_vert, skybox_frag});
		punct_program = se::make_program({punct_vert, punct_frag});
	} catch(std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
    
	se::object sun(sphere_mesh, star_program, {}, true);

	se::object planet(sphere_mesh, punct_program, {merc_diff, merc_normals, merc_amr});
	planet.set_position_callback(
		[]() -> glm::mat4 {return glm::rotate(glm::mat4(1.), time_elapsed / 300, {0, 1, 0}) * glm::translate(glm::mat4(1), glm::vec3(6.5, 0, 0));}
	);
	planet.set_scale(glm::scale(glm::mat4(1), glm::vec3(0.5)));

	se::controllable_object player(ship_mesh, punct_program, {ship_diff, ship_normals, ship_amr}, {0,0,0}, {0,0,1}, 0.05, 0.05);
	player.set_scale(glm::scale(glm::mat4(1), glm::vec3(0.02)));

	se::camera ship_camera(0.01, 2000, {1, 0, 0}, {0, 0, 0});

	player.attach_camera(ship_camera);

	se::punctual_light plight({0,0,0}, 0, 50);
	
	se::scene simple;
	simple.set_camera(ship_camera);
	simple.set_light(&plight);
	simple.set_objects({&sun, &planet, &player});
	simple.set_skybox({skybox_cubemap, skybox_program, cube_mesh});
	simple.set_exposition(3000);


    while (!glfwWindowShouldClose(window)) {
		time_elapsed = static_cast<float>(glfwGetTime());
		delta_time = time_elapsed - last_time;
		last_time = time_elapsed;
		se::s_io_processor.process_input(window, player);
		simple.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}