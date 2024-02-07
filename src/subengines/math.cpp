#include "math.hpp"

std::pair<float, glm::vec3> se::math::power_eigen(const glm::mat3& t_mat, const glm::vec3& x0) {
	// use power rule to find maximum eigenvalue and a corresponding eigenvector

	glm::vec3 x = x0;
	glm::vec3 eigenvector;
	float eigenvalue;
	for (int i = 0; i < 15; i++) {
		x = t_mat * x;
		eigenvalue = std::max(x.x, std::max(x.y, x.z));
		x /= eigenvalue;
		eigenvector = x;
	}

	return {eigenvalue, glm::normalize(eigenvector)};
}