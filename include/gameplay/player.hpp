#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>
#include "subengines/render_engine.hpp"
#include "subengines/input_engine.hpp"
#include "camera.hpp"
#include <memory>

namespace se {
	class player : public renderable, public input_listener {
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		using qu = glm::quat;
		private:
			std::unique_ptr<player_follow_camera> m_camera;
			bool should_destruct = false;
		public:
			player() = delete;
			// Giant constructor
			player(
				const v3& t_position,
				const v3& t_scale,
				const qu& t_orientation,
				const mesh& t_mesh,
				const std::list<texture>& t_textures,
				GLuint t_program
			);

			void adjust_camera() {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_camera->m_pos = m_position -8.f * direction + v3(0,1,0) * 3.f;
				m_camera->m_dir = direction;
				m_camera->rebase();
			}

			void update(input_event e) override;
			void update_mouse_offset(double x, double y) override;
			bool get_should_destruct() const { return should_destruct; }
			~player();
	
	};
}

#endif