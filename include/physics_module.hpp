#ifndef PHYSICS_MODULE_HPP
#define PHYSICS_MODULE_HPP

#include "object.hpp"
#include <vector>

namespace se {
	class collision_detector {
		private:
			std::vector<object*> m_objects;
		public:
			void set_objects(const std::vector<object*> t_objects) { m_objects = t_objects; }
			void tick();
			std::vector<object*> detect_missile_strike(const std::list<std::unique_ptr<se::missile>>& t_missiles);
	};
}

#endif