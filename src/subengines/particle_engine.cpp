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
        particle_mesh = se::load_model("../models/sphere.obj");

        GLuint particle_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/particle.vert"));
        GLuint particle_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/particle.frag"));

        program = se::make_program({particle_vert, particle_frag});

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0); // position
        glEnableVertexAttribArray(1); // color

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)0); // position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)offsetof(particle, color)); // color

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void particle_engine::render() {
        update();

        std::vector<particle_info_for_shader> instances;
        instances.reserve(particle_ptrs.size());

        for (const auto& p : particle_ptrs) {
            particle_info_for_shader instance;
            instance.position = p->position;
            instance.color = p->color;
            instances.push_back(instance);
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(particle_info_for_shader), instances.data(), GL_DYNAMIC_DRAW);

        glUseProgram(program);
        glDrawArraysInstanced(GL_TRIANGLES, 0, particle_mesh.m_indices.size(), instances.size());

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void particle_engine::update() {
        // glm::vec3 player_position = se::gameplay_engine::get_instance().get_player_position();
        glm::vec3 player_position = glm::vec3(0, 0, 2);
        make_particles(player_position, glm::normalize(glm::vec3(1, 0, 1)), glm::vec3(0, 0, -1)); // TODO: get other info on player

        particle_ptrs.remove_if([](std::unique_ptr<particle>& p) -> bool { return p->t >= p->lifetime; });

        float rate = 10; // TODO: make it dependent on player's speed
        for (auto it = particle_ptrs.begin(); it != particle_ptrs.end(); ) {
            se::particle* p = it->get();
            p->update(rate);
            ++it;
        }
    }

    void particle_engine::make_particles(glm::vec3 position, glm::vec3 incident, glm::vec3 normal) {
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);

        particle_create_info create_info;
        create_info.acceleration = glm::vec3(0, 0, -0.001);
        create_info.color = glm::vec3(1.0);
        create_info.position = position;

        for (int i = 0; i < 10; ++i) {
            float x = float(generator() % 100) / 50.f - 1.0f;
            float y = float(generator() % 100) / 50.f - 1.0f;
            float z = float(generator() % 100) / 50.f - 1.0f;

            glm::vec3 randomization = glm::vec3(x, y, z);
            glm::vec3 randomized_normal = glm::normalize(0.9f * randomization + normal);

            x = float(generator() % 100) / 10000.0f;
            glm::vec3 outgoing = x * glm::reflect(incident, randomized_normal);

            create_info.velocity = outgoing;

            x = 50.0f + float(generator() % 100) / 5.0f;
            create_info.lifetime = x;

            particle_ptrs.push_back(std::make_unique<se::particle>(&create_info));
        }
    }
}