#include "hud.hpp"
#include "clock.hpp"
#include "subengines/gameplay_engine.hpp"

extern int WINDOW_WIDTH, WINDOW_HEIGHT;

namespace se {
	static float get_cooldown_percentage(float last_time_shot, float cooldown) {
        float current_time = static_cast<float>(glfwGetTime());
        float time_passed = current_time - (last_time_shot + cooldown);
        if (time_passed > cooldown) {
            return 1.0f;
        } else if (time_passed <= .0f) {
            return 0.1f;
        } else {
            return time_passed / cooldown;
        }
    }

    void hud::drawCrosshair() {
        glUniform1i(textureUniform, 0);
        set_uniform_float(program, "alphaMod", get_cooldown_percentage(gameplay_engine::get_instance().get_last_shot_time(), gameplay_engine::get_instance().get_shooting_cooldown()));
        
        glBindVertexArray(crosshairVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, crosshairTexture.m_id);

        glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void hud::drawText(std::string text, float x, float y, float scale, glm::vec3 color) {
        glUseProgram(text_program);
        set_uniform_mat4(text_program, "projection", glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT)));
        set_uniform_vec3(text_program, "textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textVAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

    void hud::drawHealth(int percentage, float x, float y, float scale, glm::vec3 color) {
        glUseProgram(simple_program);

        float healthBarVertexArray[16] = {
            0.5f,  0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f 
        };
        unsigned int healthBarIndexArray[6] = {0, 1, 2, 1, 2, 3};
        glGenVertexArrays(1, &healthVAO);  
        glGenBuffers(1, &healthVBO);  
        glGenBuffers(1, &healthEBO);  

        glBindVertexArray(healthVAO);
        glBindBuffer(GL_ARRAY_BUFFER, healthVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(healthBarVertexArray), healthBarVertexArray, GL_STATIC_DRAW);

        glVertexAttribPointer(0, elementSize, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, healthEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), healthBarIndexArray, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        set_uniform_vec3(simple_program, "color", color);

        glBindVertexArray(healthVAO);
        glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    void hud::render() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glUseProgram(program);

        drawCrosshair();
        drawText(hud::format_points(), WINDOW_WIDTH * 0.05, WINDOW_HEIGHT * 0.9, 0.5f, green_color);
        drawText(hud::format_time(), WINDOW_WIDTH * 0.85, WINDOW_HEIGHT * 0.9, 0.5f, blue_color);
        drawHealth(100, 10, 10, 1.f, red_color);

        glUseProgram(0);

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}