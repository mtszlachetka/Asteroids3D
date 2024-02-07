#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <GL/glew.h>
#include <string>
#include <array>

namespace se
{
    struct texture
    {
        GLuint m_id;
        std::string_view m_name;
    };

    texture load_texture_2d_named(const std::string_view &filepath, const std::string_view &t_name);
    texture load_cubemap_named(const std::array<const std::string_view, 6> &paths, const std::string_view &t_name);
}

#endif