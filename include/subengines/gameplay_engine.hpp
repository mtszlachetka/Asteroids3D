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
#include "subengines/collision_engine.hpp"
#include <array>

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
			std::array<se::mesh, 4> m_asteroid_meshes;
			se::mesh m_missile_mesh;
			se::mesh m_player_mesh;
			se::mesh m_station_mesh;
			GLuint m_program; // uniform shader for all objects
			GLuint m_explosion_program; // shader for explosions
			float m_last_spawn_time = 0.f; // last asteroid spawn
			float m_last_shot_time = 0.f; // last time a missile was fired
			float m_shooting_cooldown = 0.4f;
			int m_points = 0;
			se::bounding_sphere m_player_sphere;
			se::bounding_sphere m_station_sphere;
			se::bounding_sphere m_missile_sphere;
			std::array<se::bounding_sphere, 4> m_asteroid_spheres;
			se::obb m_player_obb;
			se::obb m_station_obb;
			se::obb m_missile_obb;
			std::array<se::obb, 4> m_asteroid_obbs;
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
			int get_points() const { return m_points; }
			void add_points(int points) { m_points += points; }
			int get_player_health() { return m_player_ptr->get_health(); }
			int get_station_health() { return m_station_ptr->get_health(); }
			int get_boost() { return m_player_ptr->get_boost(); }
			float get_boost_time_used() { return m_player_ptr->get_boost_time_used(); }
			void clear();
	};
}

#endif