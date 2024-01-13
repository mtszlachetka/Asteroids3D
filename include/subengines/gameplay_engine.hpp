#ifndef GAMEPLAY_ENGINE_HPP
#define GAMEPLAY_ENGINE_HPP


#include <glm/glm.hpp>
#include <list>
#include <memory>

#include "gameplay/asteroid.hpp"
#include "gameplay/missile.hpp"
#include "gameplay/player.hpp"
#include "gameplay/station.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include <GL/glew.h>
#include <random>

namespace se {


	class gameplay_engine {
		private:
			std::list<std::unique_ptr<se::asteroid>> m_asteroid_ptrs;
			std::list<std::unique_ptr<se::missile>> m_missile_ptrs;
			std::unique_ptr<se::station> m_station_ptr;
			std::unique_ptr<se::player> m_player_ptr;
			std::list<se::texture> m_asteroid_textures;
			std::list<se::texture> m_missile_textures;
			std::list<se::texture> m_station_textures;
			std::list<se::texture> m_player_textures;
			se::mesh m_asteroid_mesh;
			se::mesh m_missile_mesh;
			se::mesh m_player_mesh;
			se::mesh m_station_mesh;
			GLuint m_program; // uniform shader for all objects
			GLuint m_explosion_program; // shader for explosions
			float m_last_spawn_time = 0.f; // last asteroid spawn
			float m_last_shot_time = 0.f; // last time a missile was fired
			float m_shooting_cooldown = 0.4f;
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
			void init();
			void tick();
			void spawn_missile();
			float get_last_shot_time() const { return m_last_shot_time; }
			float get_shooting_cooldown() const { return m_shooting_cooldown; }
			void clear();
	};
}

#endif