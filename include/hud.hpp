#ifndef HUD_HPP
#define HUD_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "object.hpp"
#include "shader.hpp"
#include "read_file.hpp"
#include "texture.hpp"

namespace se {

    class hud {
        private:
            const player* m_player;
            GLuint hudVBO;
            GLuint hudVAO; 
            GLuint hudEBO; 

            GLuint program;
            GLuint textureUniform;

            int numVertices = 4;
            int elementSize = 4;
            int numIndexes = 6;

    texture crosshair;
        public:
            void set_player(const player* t_player) { m_player = t_player; }
            void render();
            void drawHUD();
            hud() {
                crosshair = se::load_texture_2d_named("../textures/crosshair.png", "crosshair");

                GLuint hud_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/hud.vert"));
                GLuint hud_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/hud.frag"));
                program = se::make_program({hud_vert, hud_frag});

                float vertexArray[24] = {
                    .035f, .0f, .0f, 1.0f,  1.0f, 0.0f,
                    .035f,  -.12f, .0f, 1.0f,  1.0f, 1.0f,
                    -.035f, .0f, .0f, 1.0f,  0.0f, 0.0f,
                    -.035f, -.12f, .0f, 1.0f,  0.0f, 1.0f
                };

                unsigned int indexArray[6] = {0, 1, 2, 1, 2, 3};

                glGenVertexArrays(1, &hudVAO);
                glGenBuffers(1, &hudVBO);
                glGenBuffers(1, &hudEBO);

                glBindVertexArray(hudVAO);

                glBindBuffer(GL_ARRAY_BUFFER, hudVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

                glVertexAttribPointer(0, elementSize, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hudEBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), indexArray, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glBindVertexArray(0);

                textureUniform = glGetUniformLocation(program, "hudTexture");
            }
    };

}
#endif