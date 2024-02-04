#include "subengines/particle_engine.hpp"
#include "subengines/gameplay_engine.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "read_file.hpp"
#include "shader.hpp"
#include "SOIL.h"
#include "clock.hpp"
#include <memory>
#include <random>
#include <chrono>

namespace se {
    void particle_engine::init() {
        GLuint particle_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/particle.vert"));
        GLuint particle_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/particle.frag"));

        program = se::make_program({particle_vert, particle_frag});
		m_particle_texture[0] = se::load_texture_2d_named("../textures/particle_.png", "particle1");
		m_particle_texture[1] = se::load_texture_2d_named("../textures/particle_2.png", "particle2");

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &particles_vertex_buffer);
        glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(6);
        glBindBuffer(GL_ARRAY_BUFFER, particles_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glVertexAttribDivisor(0, 0);
		glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glVertexAttribDivisor(6, 0);

        glEnableVertexAttribArray(1);
        glGenBuffers(1, &particles_color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        // Initialize with empty (NULL) buffer : it will be updated later, each frame.
        glBufferData(GL_ARRAY_BUFFER, MAX_NUMBER_OF_PARTICLES * sizeof(float), NULL, GL_STREAM_DRAW);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glVertexAttribDivisor(1, 1);

        glGenBuffers(1, &particles_matrix_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_matrix_buffer);
        // Initialize with empty (NULL) buffer : it will be updated later, each frame.
        glBufferData(GL_ARRAY_BUFFER, MAX_NUMBER_OF_PARTICLES * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
        for (int i = 0; i < 4; ++i) {
            glEnableVertexAttribArray(2 + i);
            glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
            glVertexAttribDivisor(2 + i, 1);
        }

        glBindVertexArray(0);
    }

    void particle_engine::render() {
        update();

        float tex_mix_data[particle_ptrs.size()];
        glm::mat4 model_matrices[particle_ptrs.size()];

        int i = particle_ptrs.size() - 1;
        for (auto it = particle_ptrs.rbegin(); it != particle_ptrs.rend(); ++it) {
            se::particle* p = it->get();
            tex_mix_data[i] = p->texture_mix_ratio;
            model_matrices[i] = p->model_transform;
            --i;
        }

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_NUMBER_OF_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, particle_ptrs.size() * sizeof(float), tex_mix_data);

        glBindBuffer(GL_ARRAY_BUFFER, particles_matrix_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_NUMBER_OF_PARTICLES * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, particle_ptrs.size() * sizeof(glm::mat4), model_matrices);

		

        glUseProgram(program);
		set_uniform_mat4(program, "projection_matrix", se::gameplay_engine::get_instance().get_player()->get_perspective_matrix());
        set_uniform_mat4(program, "camera_matrix", se::gameplay_engine::get_instance().get_player()->get_camera_matrix());

		set_uniform_int(program, m_particle_texture[0].m_name, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_particle_texture[0].m_id);
		set_uniform_int(program, m_particle_texture[1].m_name, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_particle_texture[1].m_id);


        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particle_ptrs.size());


        glBindVertexArray(0);
        glUseProgram(0);
    }

    void particle_engine::update() {
        if (se::gameplay_engine::get_instance().get_player()->is_moving()) {
            make_particles();
        }
        float rate = 10; // TODO: make it dependent on player's speed
        for (auto it = particle_ptrs.begin(); it != particle_ptrs.end(); ) {
            se::particle* p = it->get();
            p->update(rate);
            ++it;
        }
        particle_ptrs.remove_if([](std::unique_ptr<particle>& p) -> bool { return p->t >= p->lifetime; });
    }

    void particle_engine::make_particles() {
        glm::vec3 position = se::gameplay_engine::get_instance().get_player()->get_position();
        glm::vec3 incident = glm::normalize(glm::vec3(1, 0, 1));
        glm::vec3 normal = glm::vec3(0, 0, -1);

        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);

        particle_create_info create_info;
        glm::quat player_orientation = se::gameplay_engine::get_instance().get_player()->get_orientation();
        glm::mat3 rotation = glm::toMat3(player_orientation);
        glm::vec3 direction = rotation[2];
        create_info.acceleration = glm::dot(glm::vec3(0, 0, 0.001), direction) * direction;
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_real_distribution<float> tex(0.f,1.f);
        create_info.texture_mix_ratio = tex(gen);
		position -= 1.3f * direction;
        create_info.position = position;


        int particles_to_generate = se::gameplay_engine::get_instance().get_player()->is_boosting() ? 3000 : 1000;
        for (int i = 0; i < particles_to_generate; ++i) {
            float x = float(generator() % 100) / 50.f - 1.0f;
            float y = float(generator() % 100) / 50.f - 1.0f;
            float z = float(generator() % 100) / 50.f - 1.0f;

            glm::vec3 randomization = glm::vec3(x, y, z);
            glm::vec3 randomized_normal = glm::normalize(0.9f * randomization + normal);

            x = float(generator() % 100) / 10000.0f;
            glm::vec3 outgoing = x * glm::reflect(incident, randomized_normal);

            create_info.velocity = outgoing;

            x = 10.0f + float(generator() % 100) / 5.0f;
            create_info.lifetime = x;

            particle_ptrs.push_back(std::make_unique<se::particle>(&create_info));
        }
    }
}