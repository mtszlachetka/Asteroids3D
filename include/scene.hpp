#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <GL/glew.h>
#include "light_source.hpp"
#include "object.hpp"
#include "texture.hpp"
#include "camera.hpp"

namespace se {

	struct skybox_info {
		texture m_cubemap;
		GLuint m_program;
		mesh m_mesh;
	};

	class scene {
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
			void add_object(object* t_object) { m_objects.push_back(t_object); }
			void set_light(light_source* t_light) { m_light = t_light; }
			void render();
	};

}
#endif