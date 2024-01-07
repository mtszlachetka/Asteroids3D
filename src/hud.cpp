#include "hud.hpp"

namespace se {
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

        if (static_cast<float>(glfwGetTime()) - m_player->get_last_time_shot_a_missile() > m_player->get_shooting_cooldown()) {
            drawReady();
        }

        drawCrosshair();

        glUseProgram(0);

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}
