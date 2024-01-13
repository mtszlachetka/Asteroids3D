#ifndef COLLISION_ENGINE_HPP
#define COLLISION_ENGINE_HPP

#include "gameplay/asteroid.hpp"
#include "gameplay/missile.hpp"
#include "gameplay/player.hpp"
#include "gameplay/station.hpp"

#include <list>

namespace se {
	class collision_engine {
		private:
			player* m_player;
			station* m_station;
			std::list<asteroid*> m_asteroids;
			std::list<missile*> m_missiles;
			collision_engine() {}
		public:
			collision_engine(const collision_engine& other) = delete;
			collision_engine(const collision_engine&& other) = delete;
			void operator=(const collision_engine& other) = delete;
			void operator=(const collision_engine&& other) = delete;
			static collision_engine& get_instance() {
				static collision_engine instance;
				return instance;
			}
			void attach(player* pl) { m_player = pl; }
			void attach(station* st) { m_station = st;}
			void attach(asteroid* as) { m_asteroids.push_back(as); }
			void attach(missile* mi) { m_missiles.push_back(mi); }
			void detach_player() { m_player = nullptr; }
			void detach_station() { m_station = nullptr; }
			void detach(asteroid* as) { m_asteroids.remove(as); }
			void detach(missile* mi) { m_missiles.remove(mi); }
			void tick();
	};
}

#endif