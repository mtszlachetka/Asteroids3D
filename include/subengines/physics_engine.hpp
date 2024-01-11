#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include <glm/glm.hpp>
#include <list>

#include "rigid_body.hpp"

namespace se {
	

	class physics_engine {
		private:
			std::list<rigid_body*> m_bodies;
			physics_engine() {}
		public:
			physics_engine(const physics_engine& other) = delete;
			physics_engine(const physics_engine&& other) = delete;
			void operator=(const physics_engine& other) = delete;
			void operator=(const physics_engine&& other) = delete;
			static physics_engine& get_instance() {
				static physics_engine instance;
				return instance;
			}
			void attach(rigid_body* rb) { m_bodies.push_back(rb); }
			void detach(rigid_body* rb) { m_bodies.remove(rb); }
			void tick();
	};
}

#endif