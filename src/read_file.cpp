#include "read_file.hpp"

#include <fstream>
#include <stdexcept>

namespace se {

	std::string read_file(const std::string_view& filepath) {
		std::string input;
		std::ifstream file(filepath.data(), std::ios::in);

		if (!file.good()) {
			throw std::runtime_error("Error opening file: " + std::string(filepath));
		}

		file.seekg(0, std::ios::end);
		input.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(input.data(), input.size());
		file.close();
		return input;
	}

}