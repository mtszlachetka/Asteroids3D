#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <GL/glew.h>
#include <texture.hpp>
#include <mesh.hpp>
#include <list>

#include "subengines/transformable.hpp"
#include "camera.hpp"
#include "skybox.hpp"
#include "light_source.hpp"

namespace se {
	class renderable : virtual public transformable {
		using v3 = glm::vec3;
		using qu = glm::quat;
		using m4 = glm::mat4;
		protected:
			se::mesh m_mesh;
			std::list<se::texture> m_textures;
			GLuint m_program;
		public:
			renderable();
			renderable(const v3& t_position, const v3& t_scale, const qu& t_orientation, const se::mesh& t_mesh, const std::list<se::texture>& t_textures, GLuint t_program);
			se::mesh get_mesh() const { return m_mesh; }
			std::list<se::texture> get_textures() const { return m_textures; }
			GLuint get_program() const { return m_program; }
			m4 get_model_matrix() const { return glm::translate(m4(1.f), m_position) * glm::toMat4(m_orientation) * glm::scale(m4(1.f), m_scale); }
			void set_mesh(const se::mesh& t_mesh) { m_mesh = t_mesh; }
			void set_textures(const std::list<se::texture>& t_textures) { m_textures = t_textures; }
			void set_program(GLuint t_program) { m_program = t_program; }
			virtual ~renderable();
	};

	class render_engine {
		private:
			std::list<const renderable*> m_renderables;
			const camera* m_camera = nullptr;
			const skybox* m_skybox = nullptr; // for now render engine holds a pointer to skybox - this can change later
			render_engine();
			GLuint m_shadow_map_fbo = 0;
			GLuint m_shadow_map_program = 0;
			GLuint m_shadow_map = 0;
			unsigned m_shadow_map_height = 2048;
			unsigned m_shadow_map_width = 2048;
			const light_source* m_light_source = nullptr;
			void gen_shadow_map();
			glm::mat4 m_light_space_matrix = glm::mat4(1.f);
		public:
			render_engine(const render_engine& other) = delete;
			render_engine(const render_engine&& other) = delete;
			void operator=(const render_engine& other) = delete;
			void operator=(const render_engine&& other) = delete;
			static render_engine& get_instance() {
				static render_engine instance;
				return instance;
			}
			void attach(const renderable* rd) { m_renderables.push_back(rd); }
			void detach(const renderable* rd) { m_renderables.remove(rd); }
			void tick();
			void set_camera(const camera* t_camera) { m_camera = t_camera; }
			void set_skybox(const skybox* sk) { m_skybox = sk; }
			void set_light(const light_source* ls) { 
				m_light_source = ls; 
				m_light_space_matrix = glm::ortho(-30.f, 30.f, -30.f, 30.f, -30.f, 60.f) * glm::lookAt(m_light_source->get_position(), glm::vec3(0.f), glm::vec3(0, 1, 0)); 
			}
	};
}

#endif