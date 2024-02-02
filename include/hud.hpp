#ifndef HUD_HPP
#define HUD_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map> 
#include <iostream>
#include "shader.hpp"
#include "read_file.hpp"
#include "texture.hpp"
#include "subengines/gameplay_engine.hpp"

namespace se {
    struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
    };

    class hud {
        private:
            GLuint crosshairVBO, crosshairVAO, crosshairEBO;
            GLuint textVAO, textVBO;

            GLuint stationHealthOutlineVAO, stationHealthOutlineVBO, stationHealthOutlineEBO;
            GLuint stationHealthVAO, stationHealthVBO, stationHealthEBO;

            GLuint boostOutlineVAO, boostOutlineVBO, boostOutlineEBO;
            GLuint boostVAO, boostVBO, boostEBO;

            GLuint playerHealthOutlineVAO, playerHealthOutlineVBO, playerHealthOutlineEBO;
            GLuint playerHealthVAO, playerHealthVBO, playerHealthEBO;

            GLuint program;
            GLuint text_program;
            GLuint simple_program;

            GLuint textureUniform;

            glm::vec3 green_color = glm::vec3(0.12, 1.f, 0.f);
            glm::vec3 blue_color = glm::vec3(0.2f, 0.4f, 1.f);
            glm::vec3 red_color = glm::vec3(1.f, 0.f, 0.12f);  

            int numVertices = 4;
            int elementSize = 4;
            int numIndexes = 6;

            texture crosshairTexture;

            std::map<char, Character> Characters;

            float stationHealthBarVertexArray[16] = {
                -0.2f,  -0.9f, 0.0f, 1.0f,
                -0.25f, -0.95f, 0.0f, 1.0f,
                -0.95f, -0.95f, 0.0f, 1.0f,
                -0.9f,  -0.9f, 0.0f, 1.0f 
            };

            float boostBarVertexArray[16] = {
                -0.3f,  -0.8f, 0.0f, 1.0f,
                -0.35f, -0.85f, 0.0f, 1.0f,
                -0.95f, -0.85f, 0.0f, 1.0f,
                -0.9f,  -0.8f, 0.0f, 1.0f 
            };

            float healthBarVertexArray[16] = {
                -0.4f,  -0.7f, 0.0f, 1.0f,
                -0.45f, -0.75f, 0.0f, 1.0f,
                -0.95f, -0.75f, 0.0f, 1.0f,
                -0.9f,  -0.7f, 0.0f, 1.0f 
            };

            unsigned int barsIndexArray[6] = {0, 1, 3, 1, 2, 3};

            int currentPoints = 0;
            int updatedPoints = 0;

            int currentStationHealth = 100;
            int updatedStationHealth = 100;

            int currentBoost = 100;
            int updatedBoost = 100;

            int currentPlayerHealth = 100;
            int updatedPlayerHealth = 100;
			
            void drawCrosshair();
            void drawText(std::string text, float x, float y, float scale, glm::vec3 color);
            void drawStationHealth(int percentage, glm::vec3 color);
            void drawBoost(int percentage, glm::vec3 color);
            void drawPlayerHealth(int percentage, glm::vec3 color);
        public:
            void render();
            hud() {

                GLuint text_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/text.vert"));
                GLuint text_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/text.frag"));
                text_program = se::make_program({text_vert, text_frag});

                FT_Library ft;
                FT_Face face;

                if (FT_Init_FreeType(&ft))
                {
                    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

                }

                if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face))
                {
                    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
                }

                FT_Set_Pixel_Sizes(face, 0, 64);      

                glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
                for (unsigned char c = 0; c < 128; c++)
                {
                    // load character glyph 
                    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                    {
                        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                        continue;
                    }
                    // generate texture
                    unsigned int texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                    );
                    // set texture options
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    // now store character for later use
                    Character character = {
                        texture, 
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        static_cast<unsigned int>(face->glyph->advance.x)
                    };
                    Characters.insert(std::pair<char, Character>(c, character));
                }

                FT_Done_Face(face);
                FT_Done_FreeType(ft);

                glGenVertexArrays(1, &textVAO);
                glGenBuffers(1, &textVBO);
                glBindVertexArray(textVAO);
                glBindBuffer(GL_ARRAY_BUFFER, textVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);  

                // ---------------------------------------------------------------------------------------------

                crosshairTexture = se::load_texture_2d_named("../textures/crosshair.png", "crosshair");

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

                glBindVertexArray(0);

                textureUniform = glGetUniformLocation(program, "hudTexture");

                // ---------------------------------------------------------------------------------------------

                GLuint simple_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/simple.vert"));
                GLuint simple_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/simple.frag"));
                simple_program = se::make_program({simple_vert, simple_frag});


                float healthBarVertexArray[16] = {
                    -0.2f,  -0.9f, 0.0f, 1.0f,
                    -0.25f, -0.95f, 0.0f, 1.0f,
                    -0.95f, -0.95f, 0.0f, 1.0f,
                    -0.9f,  -0.9f, 0.0f, 1.0f 
                };

                glGenVertexArrays(1, &stationHealthOutlineVAO);  
                glGenBuffers(1, &stationHealthOutlineVBO);  
                glGenBuffers(1, &stationHealthOutlineEBO);  

                glBindVertexArray(stationHealthOutlineVAO);
                glBindBuffer(GL_ARRAY_BUFFER, stationHealthOutlineVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(healthBarVertexArray), healthBarVertexArray, GL_STATIC_DRAW);

                glVertexAttribPointer(0, elementSize, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(4 * sizeof(float)));
                glEnableVertexAttribArray(1);
            
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stationHealthOutlineEBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), barsIndexArray, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                float boostBarVertexArray[16] = {
                    -0.3f,  -0.8f, 0.0f, 1.0f,
                    -0.35f, -0.85f, 0.0f, 1.0f,
                    -0.95f, -0.85f, 0.0f, 1.0f,
                    -0.9f,  -0.8f, 0.0f, 1.0f 
                };

                glGenVertexArrays(1, &boostOutlineVAO);  
                glGenBuffers(1, &boostOutlineVBO);  
                glGenBuffers(1, &boostOutlineEBO);  

                glBindVertexArray(boostOutlineVAO);
                glBindBuffer(GL_ARRAY_BUFFER, boostOutlineVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(boostBarVertexArray), boostBarVertexArray, GL_STATIC_DRAW);

                glVertexAttribPointer(0, elementSize, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(4 * sizeof(float)));
                glEnableVertexAttribArray(1);
            
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boostOutlineEBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), barsIndexArray, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, 0);


                float playerBarVertexArray[16] = {
                    -0.4f,  -0.7f, 0.0f, 1.0f,
                    -0.45f, -0.75f, 0.0f, 1.0f,
                    -0.95f, -0.75f, 0.0f, 1.0f,
                    -0.9f,  -0.7f, 0.0f, 1.0f 
                };

                glGenVertexArrays(1, &playerHealthOutlineVAO);  
                glGenBuffers(1, &playerHealthOutlineVBO);  
                glGenBuffers(1, &playerHealthOutlineEBO);  

                glBindVertexArray(playerHealthOutlineVAO);
                glBindBuffer(GL_ARRAY_BUFFER, playerHealthOutlineVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(playerBarVertexArray), playerBarVertexArray, GL_STATIC_DRAW);

                glVertexAttribPointer(0, elementSize, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(4 * sizeof(float)));
                glEnableVertexAttribArray(1);
            
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerHealthOutlineEBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), barsIndexArray, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

            }
            std::string format_points(int points) {
                std::string points_str = std::to_string(points);
                std::string formatted_points = "";
                for (int i = 0; i < 6 - points_str.length(); i++) {
                    formatted_points += "0";
                }
                formatted_points += points_str;
                return formatted_points;
            }
            std::string format_time() {
                float time = game_clock::get_instance().get_current_frame_time();
                int hours = static_cast<int>(time / 3600);
                int minutes = static_cast<int>(time / 60);
                int seconds = static_cast<int>(time) % 60;
                std::string hours_str = std::to_string(hours);
                std::string minutes_str = std::to_string(minutes);
                std::string seconds_str = std::to_string(seconds);
                std::string formatted_time = "";
                for (int i = 0; i < 2 - hours_str.length(); i++) {
                    formatted_time += "0";
                }
                formatted_time += hours_str;
                formatted_time += ":";
                for (int i = 0; i < 2 - minutes_str.length(); i++) {
                    formatted_time += "0";
                }
                formatted_time += minutes_str;
                formatted_time += ":";
                for (int i = 0; i < 2 - seconds_str.length(); i++) {
                    formatted_time += "0";
                }
                formatted_time += seconds_str;
                return formatted_time;
            }
            float calculate_value(float min, float max, int percentage) {
                float percent = percentage / 100.0f;
                float range = max - min;
                float value = min + percent * range;
                if (value < min) {
                    return min;
                }
                return value;
            }
    };

}
#endif