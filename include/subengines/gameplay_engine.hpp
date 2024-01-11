#ifndef GAMEPLAY_ENGINE_HPP
#define GAMEPLAY_ENGINE_HPP

#include "gameplay_object.hpp"
#include "gameplay/asteroid.hpp"
#include "gameplay/missile.hpp"
#include "gameplay/player.hpp"
#include "gameplay/station.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <list>

namespace se {

	enum class gameplay_event {
		asteroid_destroyed,
		missile_destroyed,
		station_destroyed,
		player_destroyed
	};

	class gameplay_engine {
		private:
			std::list<gameplay_object*> m_objects;
			gameplay_engine() {}
		public:
			gameplay_engine(const gameplay_engine& other) = delete;
			gameplay_engine(const gameplay_engine&& other) = delete;
			void operator=(const gameplay_engine& other) = delete;
			void operator=(const gameplay_engine&& other) = delete;
			static gameplay_engine& get_instance() {
				static gameplay_engine instance;
				return instance;
			}
			void spawn_asteroid();
			void init();
			void run();
	};
}

#endif