#include "hud.hpp"
#include "clock.hpp"
#include "subengines/gameplay_engine.hpp"

extern int WINDOW_WIDTH, WINDOW_HEIGHT;

namespace se {
    void hud::drawText(std::string text, float x, float y, float scale, glm::vec3 color)
    {
        glUseProgram(text_program);
        set_uniform_mat4(text_program, "projection", glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT)));
        glUniform3f(glGetUniformLocation(text_program, "textColor"), color.x, color.y, color.z);
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


    void hud::drawCrosshair() {
        glUniform1i(textureUniform, 0);

        glBindVertexArray(crosshairVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, crosshairTexture.m_id);

        glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void hud::drawReady() {
        glUniform1i(textureUniform, 1);

        glBindVertexArray(readyVAO);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, readyTexture.m_id);

        glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void hud::render() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glUseProgram(program);

        if (game_clock::get_instance().get_current_frame_time() - gameplay_engine::get_instance().get_last_shot_time() > gameplay_engine::get_instance().get_shooting_cooldown()) {
            drawReady();
        }

        drawCrosshair();
        drawText(hud::format_points(), WINDOW_WIDTH * 0.05, WINDOW_HEIGHT * 0.9, 0.5f, glm::vec3(0.12, 1.f, 0.f));

        glUseProgram(0);

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}
