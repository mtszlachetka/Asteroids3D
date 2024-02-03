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
    struct particle_info_for_shader {
        glm::vec3 position;
        glm::vec3 color;
    };

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

                t += rate;
                tint = glm::vec4(color, 1.0f - (t / lifetime));
            }
    };

    class particle_engine {
        private:
            se::mesh particle_mesh;
            GLuint program;
            GLuint vao, vbo;
            std::list<std::unique_ptr<se::particle>> particle_ptrs;
            void make_particles(glm::vec3 position, glm::vec3 incident, glm::vec3 normal);
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