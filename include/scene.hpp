#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <vector>
#include <GL/glew.h>
#include "light_source.hpp"
#include "object.hpp"
#include "texture_manager.hpp"
#include "camera.hpp"

namespace se {

	struct skybox_info {
		texture_info m_cubemap;
		GLuint m_program;
		mesh m_mesh;
	};

	class scene {
		friend class renderer;
		private:
			float m_exposition;
			std::vector<object*> m_objects;
			light_source* m_light;
			const camera* m_cam;
			skybox_info m_skybox;
		public:
			void set_exposition(float e) { m_exposition = e; }
			void set_skybox(skybox_info t_skybox) { m_skybox = t_skybox; }
			void set_camera(const camera& t_cam) { m_cam = &t_cam; }
			void set_objects(const std::vector<object*>& t_objects) { m_objects = t_objects; }
			void set_light(light_source* t_light) { m_light = t_light; }
			void render();
	};

}
#endif