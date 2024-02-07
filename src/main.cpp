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
#include "gameplay/station.hpp"
#include "subengines/physics_engine.hpp"
#include "subengines/render_engine.hpp"
#include "subengines/input_engine.hpp"
#include "subengines/gameplay_engine.hpp"
#include "subengines/collision_engine.hpp"
#include "subengines/particle_engine.hpp"
#include "skybox.hpp"
#include "light_source.hpp"
#include "hud.hpp"

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;
float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); 
	WINDOW_WIDTH = width; 
	WINDOW_HEIGHT = height; 
	ASPECT_RATIO = (float)width / (float)height;
	se::render_engine::get_instance().update_framebuffer();
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
    se::mesh cube_mesh = se::load_model("../models/cube.obj");

    // // cubemaps
    std::array<const std::string_view, 6> walls = {
        "../textures/skybox/space_rt.png", 
        "../textures/skybox/space_lf.png", 
        "../textures/skybox/space_up.png", 
        "../textures/skybox/space_dn.png", 
        "../textures/skybox/space_bk_sun_dark.png", 
        "../textures/skybox/space_ft_galaxy.png"
    };
    se::texture skybox_cubemap = se::load_cubemap_named(walls, "skybox");

    // // shader creation	
    GLuint skybox_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/skybox.vert"));
    GLuint skybox_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/skybox.frag"));
	
	GLuint skybox_program;
	try {
		skybox_program = se::make_program({skybox_vert, skybox_frag});
	} catch(std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	se::game_clock& clock = se::game_clock::get_instance();

	se::physics_engine& pe = se::physics_engine::get_instance();
	se::render_engine& re = se::render_engine::get_instance();
	se::input_engine& ie = se::input_engine::get_instance();
	se::gameplay_engine& ge = se::gameplay_engine::get_instance();
	se::collision_engine& ce = se::collision_engine::get_instance();
	se::particle_engine& particle_engine = se::particle_engine::get_instance();

	ie.set_active_window(window);

	se::skybox skybox(skybox_cubemap, cube_mesh, skybox_program);
	se::light_source sunlight({0,0,30});

	re.set_skybox(&skybox);
	re.set_light(&sunlight);

	ge.init();
	particle_engine.init();

	se::hud hud;
	se::texture menu_texture = se::load_texture_2d_named("../textures/skybox/space_dn.png", "");

	// wait for the player to press ENTER
	while (1) {
		hud.drawInitialText(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.5, menu_texture.m_id);
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) break;
	}

	// se::free_camera free(0.01, 200, {0,0,1}, {0,0,0});
	// re.set_camera(&free);
	clock.init();

    while (!glfwWindowShouldClose(window)) {
		clock.tick();
		ie.tick();
		ge.tick();
		pe.tick();
		ce.tick();
		particle_engine.update();
		re.tick();
		hud.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	ge.clear();


    glfwTerminate();
	
}