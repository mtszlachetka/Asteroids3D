#include "subengines/debug.hpp"

void se::debug::print_mesh_info(const mesh &t_mesh)
{
	// std::clog << "number of vertices: " << t_mesh.m_vertices.size() << "\n";

	std::clog << "vertex data:\n";
	unsigned i = 0;
	for (const vertex &v : t_mesh.m_vertices)
	{
		std::clog
			<< ++i << "\n"
			<< "\nposition:\t" << v.m_position.x << "\t" << v.m_position.y << "\t" << v.m_position.z << "\n"
			<< "normal:\t" << v.m_normal.x << "\t" << v.m_normal.y << "\t" << v.m_normal.z << "\n"
			<< "tex_coord:\t" << v.m_tex_coords.x << "\t" << v.m_tex_coords.y << "\n"
			<< "tangent:\t" << v.m_tangent.x << "\t" << v.m_tangent.y << "\t" << v.m_tangent.z << "\n"
			<< "bitangent:\t" << v.m_bitangent.x << "\t" << v.m_bitangent.y << "\t" << v.m_bitangent.z << "\n";
	}
	// std::clog << "indices:\n\n";
	// for (int i = 0; i < t_mesh.m_indices.size(); i+=3) {
	// 	std::clog << t_mesh.m_indices[i] << " " << t_mesh.m_indices[i+1] << " " << t_mesh.m_indices[i+2] << "\n";
	// }
}
