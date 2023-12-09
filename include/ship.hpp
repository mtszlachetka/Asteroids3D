#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "object.hpp"
#include <iostream>
#include <glm/ext.hpp>

namespace SE {

	class ship : public object {
		using v3 = glm::vec3;
		friend class io_processor;
		private:
			v3 m_pos, m_dir, m_side, m_up;
			float m_movespeed, m_anglespeed;
			glm::mat4 get_rotation_matrix() const {
				return {
					m_side.x, m_up.x, m_dir.x, 0,
					m_side.y, m_up.y, m_dir.y, 0,
					m_side.z, m_up.z, m_dir.z, 0,
					0, 0, 0, 1
				};
			}
		public:
			ship(const SE::mesh_info& t_mesh, GLuint t_program, const std::vector<SE::texture_info>& tex, const v3& t_pos, const v3& t_dir, 
					float mspeed, float aspeed) : object(t_mesh, t_program, tex), m_pos(t_pos), m_dir(t_dir), 
						m_movespeed(mspeed), m_anglespeed(aspeed) { rebase(); }
			virtual glm::mat4 get_position(float time) const { return glm::translate(glm::mat4(1.0), m_pos) * get_rotation_matrix() * glm::scale(glm::mat4(1), glm::vec3(0.002)); }
			void rebase() {
				m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
				m_up = glm::normalize(glm::cross(m_side, m_dir));
			}
			~ship() {}
	};
}


#endif