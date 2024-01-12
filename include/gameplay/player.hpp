#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>
#include "subengines/render_engine.hpp"
#include "subengines/input_engine.hpp"

namespace se {
	class player : public renderable, public input_listener {
		using v3 = glm::vec3;
		using qu = glm::quat;
		public:
			player() = default;
			// Giant constructor
			player(
				const v3& t_position,
				const v3& t_scale,
				const qu& t_orientation,
				const mesh& t_mesh,
				const std::list<texture>& t_textures,
				GLuint t_program
			) : transformable(t_position, t_scale, t_orientation),
				renderable(t_position, t_scale, t_orientation, t_mesh, t_textures, t_program),
				input_listener() {}

			void update(input_event e) override {}
	};
}

#endif