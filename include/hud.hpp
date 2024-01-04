#ifndef HUD_HPP
#define HUD_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "object.hpp"

namespace se {

    class hud {
        private:
            const player* m_player;
        public:
            void set_player(const player* t_player) { m_player = t_player; }
            void render();
            void initializeHUD();
    };

}
#endif