#ifndef PARTICLE_ENGINE_HPP
#define PARTICLE_ENGINE_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>
#include <memory>
#include "shader.hpp"
#include "read_file.hpp"
#include "texture.hpp"
#include "subengines/gameplay_engine.hpp"

namespace se {
    struct particle_create_info {
        glm::vec3 position, velocity, acceleration;
        float lifetime;
        glm::vec3 color;
    };

    class particle {
        public:
            glm::vec3 position, velocity, acceleration;
            float t, lifetime;
            glm::vec3 color;
            glm::vec4 tint;
            glm::mat4 model_transform;
            particle(particle_create_info* create_info) {
                this->position = create_info->position;
                this->velocity = create_info->velocity;
                this->acceleration = create_info->acceleration;
                this->color = create_info->color;
                this->lifetime = create_info->lifetime;
                t = 0.0f;
                model_transform = glm::mat4(1.0f);
                model_transform = glm::translate(model_transform, position);
            }
            void update(float rate) {
                velocity += rate * acceleration;
                position += rate * velocity;

                model_transform = glm::mat4(1.0f);
                model_transform = glm::translate(model_transform, position);
                model_transform = glm::scale(model_transform, glm::vec3(0.1f));

                t += rate;
                tint = glm::vec4(color, 1.0f - (t / lifetime));
            }
    };

    class particle_engine {
        private:
            GLuint program;
            GLuint vao;
            static constexpr const GLfloat vertex_buffer_data[] = {
                0.5f, 0.5f, 0.0f, 1.f, 0.f,
                0.5f, -0.5f, 0.0f, 1.f, 1.f,
                -0.5f, 0.5f, 0.0f, 0.f, 0.f,
                -0.5f, -0.5f, 0.0f, 0.f, 1.f
            };
            GLuint particles_vertex_buffer;
            static const int MAX_NUMBER_OF_PARTICLES = 100000;
            GLuint particles_color_buffer;
            GLuint particles_matrix_buffer;
            std::list<std::unique_ptr<se::particle>> particle_ptrs;
			se::texture m_particle_texture;
            void make_particles();
            void update();
            particle_engine() {}
        public:
			particle_engine(const particle_engine& other) = delete;
			particle_engine(const particle_engine&& other) = delete;
			void operator=(const particle_engine& other) = delete;
			void operator=(const particle_engine&& other) = delete;
			static particle_engine& get_instance() {
				static particle_engine instance;
				return instance;
			}
			void init();
            void render();
    };
}
#endif