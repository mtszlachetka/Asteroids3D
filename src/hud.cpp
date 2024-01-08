#include "hud.hpp"
#include "clock.hpp"
#include "subengines/gameplay_engine.hpp"

namespace se {
    void hud::drawCrosshair() {
        glUniform1i(textureUniform, 0);
        glUniform1f(glGetUniformLocation(program, "alphaMod"), 1.0f);

        glBindVertexArray(crosshairVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, crosshairTexture.m_id);

        glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    float get_cooldown_percentage(float last_time_shot, float cooldown) {
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

    void hud::drawReady() {
        glUniform1i(textureUniform, 1);
        glUniform1f(glGetUniformLocation(program, "alphaMod"), get_cooldown_percentage(gameplay_engine::get_instance().get_last_shot_time(), gameplay_engine::get_instance().get_shooting_cooldown()));

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

        drawReady();
        drawCrosshair();

        glUseProgram(0);

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}