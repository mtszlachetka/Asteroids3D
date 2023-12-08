#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <vector>
#include "light_source.hpp"
#include "object.hpp"
#include "texture_manager.hpp"
#include "camera.hpp"

namespace SE {

	struct skybox_info {
		texture_info cubemap;
		GLuint program;
		mesh_info mesh;
	};

	class scene {
		friend class renderer;
		private:
			float m_exposition;
			std::vector<object*> m_objects;
			std::vector<light_source> m_light_sources;
			camera* m_cam;
			skybox_info m_skybox;
		public:
			void set_exposition(float e) { m_exposition = e; }
			void set_skybox(skybox_info t_skybox) { m_skybox = t_skybox; }
			void set_camera(camera& t_cam) { m_cam = &t_cam; }
			void set_objects(const std::vector<object*>& t_objects) { m_objects = t_objects; }
			void set_light_sources(const std::vector<light_source>& t_light_sources) { m_light_sources = t_light_sources; }
	};

}
#endif