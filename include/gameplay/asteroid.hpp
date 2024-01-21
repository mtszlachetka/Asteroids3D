#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "subengines/render_engine.hpp"
#include "subengines/physics_engine.hpp"

namespace se {
	class asteroid : public renderable, public rigid_body {
		using v3 = glm::vec3;
		using qu = glm::quat;
		private:
			float time_of_destruction = 0.f;
		public:
			asteroid() = delete;
			// Giant constructor
			asteroid(
				const v3& t_position,
				const v3& t_scale,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				GLuint t_program,
				const v3& t_velocity,
				float t_mass
			);
			virtual ~asteroid();
			void notify_missile_collision() {
				time_of_destruction = game_clock::get_instance().get_current_frame_time();
				this->set_time_of_destruction(time_of_destruction);
			}
			void notify_asteroid_collision(const v3& other_velocity, float other_mass);
			void notify_station_collision() { 
				time_of_destruction = game_clock::get_instance().get_current_frame_time(); 
				this->set_time_of_destruction(time_of_destruction);
			}
			bool get_should_destruct() const { 
				if (time_of_destruction != 0.0) {
					if (game_clock::get_instance().get_current_frame_time() - time_of_destruction > 3.f) {
						return true;
					}
				}
				return false;
			}
			float get_time_of_destruction() const { return time_of_destruction; }
	};
}

#endif