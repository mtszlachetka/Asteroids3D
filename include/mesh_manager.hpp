#ifndef _MESH_MANAGER_HPP_
#define _MESH_MANAGER_HPP_

#include <GL/glew.h>
#include <string>

namespace SE {
	
	struct mesh_info {
		GLuint vertex_array;
		GLuint vertex_buffer;
		GLuint vertex_index_buffer;
		unsigned size;
	};

	class mesh_manager {
		public:
			mesh_info load(const std::string& path);
	};

	static mesh_manager s_mesh_manager;
}



#endif
