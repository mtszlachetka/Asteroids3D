#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <string>

namespace se {
	
	struct mesh {
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_ebo;
		unsigned m_size;
		void render() {
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
	};

	mesh load_model(const std::string_view& filepath);

}



#endif
