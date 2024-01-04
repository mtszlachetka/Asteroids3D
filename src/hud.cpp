#include "hud.hpp"
#include "shader.hpp"
#include "read_file.hpp"
#include "texture.hpp"

extern int WINDOW_WIDTH, WINDOW_HEIGHT;

namespace se {
    GLuint hudVBO;
    GLuint hudVAO; 
    GLuint hudEBO; 

    GLuint program;
    GLuint textureUniform;

    int numVertices = 4;
    int elementSize = 4;
    int numIndexes = 6;

    texture crosshair;

    void hud::initializeHUD() {
        crosshair = se::load_texture_2d_named("../textures/crosshair.png", "crosshair");

        GLuint hud_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/hud.vert"));
        GLuint hud_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/hud.frag"));
        program = se::make_program({hud_vert, hud_frag});

        float vertexArray[24] = {
            -.05f, .05f, .0f, 1.0f,  0.0f, 1.0f,
            .05f,  -.05f, .0f, 1.0f,  1.0f, 0.0f,
            .05f, .05f, .0f, 1.0f,  1.0f, 1.0f,
            -.05f, -.05f, .0f, 1.0f,  0.0f, 0.0f
        };

        unsigned int indexArray[6] = {0, 1, 2, 0, 2, 3};

        glGenVertexArrays(1, &hudVAO);
        glGenBuffers(1, &hudVBO);
        glGenBuffers(1, &hudEBO);

        glBindVertexArray(hudVAO);

        glBindBuffer(GL_ARRAY_BUFFER, hudVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * elementSize * sizeof(float), vertexArray, GL_STATIC_DRAW);

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

    void drawHUD() {
        glUniform1i(textureUniform, 0);

        glBindVertexArray(hudVAO);

        glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void hud::render() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glUseProgram(program);

        drawHUD();

        glUseProgram(0);

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}
