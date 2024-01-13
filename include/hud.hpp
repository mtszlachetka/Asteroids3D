#ifndef HUD_HPP
#define HUD_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "shader.hpp"
#include "read_file.hpp"
#include "texture.hpp"

namespace se {

    class hud {
        private:
            GLuint crosshairVBO;
            GLuint crosshairVAO; 
            GLuint crosshairEBO; 

            GLuint readyVBO;
            GLuint readyVAO; 
            GLuint readyEBO; 

            GLuint program;
            GLuint textureUniform;

            int numVertices = 4;
            int elementSize = 4;
            int numIndexes = 6;

            texture crosshairTexture;
            texture readyTexture;
			
            void drawCrosshair();
            void drawReady();
        public:
            void render();
            hud() {
                crosshairTexture = se::load_texture_2d_named("../textures/crosshair.png", "crosshair");
                readyTexture = se::load_texture_2d_named("../textures/ready.png", "ready");

                GLuint hud_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/hud.vert"));
                GLuint hud_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/hud.frag"));
                program = se::make_program({hud_vert, hud_frag});

                float crosshairVertexArray[24] = {
                    .035f, .0f, .0f, 1.0f,  1.0f, 0.0f,
                    .035f,  -.12f, .0f, 1.0f,  1.0f, 1.0f,
                    -.035f, .0f, .0f, 1.0f,  0.0f, 0.0f,
                    -.035f, -.12f, .0f, 1.0f,  0.0f, 1.0f
                };

                unsigned int crosshairIndexArray[6] = {0, 1, 2, 1, 2, 3};

                glGenVertexArrays(1, &crosshairVAO);
                glGenBuffers(1, &crosshairVBO);
                glGenBuffers(1, &crosshairEBO);

                glBindVertexArray(crosshairVAO);

                glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertexArray), crosshairVertexArray, GL_STATIC_DRAW);

                glVertexAttribPointer(0, elementSize, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crosshairEBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), crosshairIndexArray, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, 0);


                float readyVertexArray[24] = {
                    .3f, .8f, .0f, 1.0f,  1.0f, 0.0f,
                    .3f,  .6f, .0f, 1.0f,  1.0f, 1.0f,
                    -.3f, .8f, .0f, 1.0f,  0.0f, 0.0f,
                    -.3f, .6f, .0f, 1.0f,  0.0f, 1.0f
                };

                unsigned int readyIndexArray[6] = {0, 1, 2, 1, 2, 3};

                glGenVertexArrays(1, &readyVAO);
                glGenBuffers(1, &readyVBO);
                glGenBuffers(1, &readyEBO);

                glBindVertexArray(readyVAO);

                glBindBuffer(GL_ARRAY_BUFFER, readyVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(readyVertexArray), readyVertexArray, GL_STATIC_DRAW);

                glVertexAttribPointer(0, elementSize, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, readyEBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), readyIndexArray, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glBindVertexArray(0);

                textureUniform = glGetUniformLocation(program, "hudTexture");
            }
    };

}
#endif