#ifndef MATH_HPP
#define MATH_HPP

#include <utility>
#include <glm/glm.hpp>

namespace se {
namespace math {

	std::pair<float, glm::vec3> power_eigen(const glm::mat3& t_mat, const glm::vec3& x0);

}
}

#endif