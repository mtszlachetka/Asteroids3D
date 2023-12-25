#ifndef READ_FILE_HPP
#define READ_FILE_HPP

#include <string>

namespace se {
	std::string read_file(const std::string_view& filepath);
}

#endif