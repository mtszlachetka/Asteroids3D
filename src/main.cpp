#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "read_file.hpp"
#include "clock.hpp"
#include "gameplay/asteroid.hpp"
#include "subengines/collision_engine.hpp"

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;
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
	glClearColor(0.3,0.3,0.3,1);

    // model loading
    se::mesh ship_mesh = se::load_model("../models/spaceship.obj");
    se::mesh sphere_mesh = se::load_model("../models/sphere.obj");
    se::mesh cube_mesh = se::load_model("../models/cube.obj");
	se::mesh missile_mesh = se::load_model("../models/missile.obj");


	// // textures for PBR
	se::texture ship_diff = se::load_texture_2d_named("../textures/spaceshipPBR/diff.png", "diffuse_map");
	se::texture ship_normals = se::load_texture_2d_named("../textures/spaceshipPBR/norm.png", "normal_map");
	se::texture ship_amr = se::load_texture_2d_named("../textures/spaceshipPBR/amr.png", "amr_map");

	se::texture merc_diff = se::load_texture_2d_named("../textures/rock/diff.jpg", "diffuse_map");
	se::texture merc_normals = se::load_texture_2d_named("../textures/rock/norm.jpg", "normal_map");
	se::texture merc_amr = se::load_texture_2d_named("../textures/rock/arm.jpg", "amr_map");

	se::texture missile_diff = se::load_texture_2d_named("../textures/missile/diff.png", "diffuse_map");
	se::texture missile_normals = se::load_texture_2d_named("../textures/missile/norm.png", "normal_map");
	se::texture missile_amr = se::load_texture_2d_named("../textures/missile/amr.png", "amr_map");

    // // cubemaps
    std::array<const std::string_view, 6> walls = {
        "../textures/skybox/space_rt.png", 
        "../textures/skybox/space_lf.png", 
        "../textures/skybox/space_up.png", 
        "../textures/skybox/space_dn.png", 
        "../textures/skybox/space_bk.png", 
        "../textures/skybox/space_ft_galaxy.png"
    };
    se::texture skybox_cubemap = se::load_cubemap_named(walls, "skybox");

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
    

	se::game_clock& clock = se::game_clock::get_instance();

    while (!glfwWindowShouldClose(window)) {
		clock.tick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}