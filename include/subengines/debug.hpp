#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <GL/glew.h>
#include <cstring>
#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include <iostream>
namespace se
{
	namespace debug
	{

		class texture_drawer
		{
		private:
			GLuint banner_vao, banner_vbo, banner_ebo;
			GLuint program;
			static constexpr const GLfloat vertices[] = {
				// vertex coords		// tex coords
				1.f, 1.f, 0.f, 1.f, 0.f,
				1.f, -1.f, 0.f, 1.f, 1.f,
				-1.f, 1.f, 0.f, 0.f, 0.f,
				-1.f, -1.f, 0.f, 0.f, 1.f};
			static constexpr const GLuint indices[] = {0, 1, 2, 1, 2, 3};
			static constexpr const std::string_view vsrc =
				"#version 430 core\n"
				"layout(location = 0) in vec3 vertex_position;\n"
				"layout(location = 1) in vec2 vertex_tex_coord;\n"
				"out vec2 tex_coord;\n"
				"void main() {\n"
				"gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);\n"
				"tex_coord = vertex_tex_coord;\n"
				"}\n\0";
			static constexpr const std::string_view fsrc =
				"#version 430 core\n"
				"uniform sampler2D tex;\n"
				"in vec2 tex_coord;\n"
				"out vec4 out_color;\n"
				"void main() {\n"
				"out_color = texture(tex, tex_coord);\n"
				"}\n\0";
			void compile_shaders()
			{
				GLuint vertex_shader = shader_from_string(GL_VERTEX_SHADER, vsrc);
				GLuint fragment_shader = shader_from_string(GL_FRAGMENT_SHADER, fsrc);

				program = make_program({vertex_shader, fragment_shader});

				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
			}
			void init_banner()
			{
				glGenVertexArrays(1, &banner_vao);
				glBindVertexArray(banner_vao);

				glGenBuffers(1, &banner_vbo);
				glBindBuffer(GL_ARRAY_BUFFER, banner_vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				glGenBuffers(1, &banner_ebo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, banner_ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

				glBindVertexArray(0);
			}

		public:
			texture_drawer()
			{
				compile_shaders();
				init_banner();
			}
			void draw(GLuint id)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glUseProgram(program);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, id);
				glBindVertexArray(banner_vao);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		};

		void print_mesh_info(const mesh &t_mesh);

		inline void print_vector3(const glm::vec3 &vec)
		{
			std::clog.precision(5);
			std::clog.setf(std::ios_base::fixed);
			std::clog << "[" << vec.x << " " << vec.y << " " << vec.z << "]\n";
		}
		inline void print_mat3(const glm::mat3 &mat)
		{
			print_vector3(mat[0]);
			print_vector3(mat[1]);
			print_vector3(mat[2]);
		}
	}
}

#endif