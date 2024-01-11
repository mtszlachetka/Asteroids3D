#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <GL/glew.h>
#include <texture.hpp>
#include <mesh.hpp>
#include <list>

#include "renderable.hpp"

namespace se {
	

	class render_engine {
		private:
			std::list<const renderable*> m_renderables;
			render_engine() {}
		public:
			render_engine(const render_engine& other) = delete;
			render_engine(const render_engine&& other) = delete;
			void operator=(const render_engine& other) = delete;
			void operator=(const render_engine&& other) = delete;
			static render_engine& get_instance() {
				static render_engine instance;
				return instance;
			}
			void attach(const renderable* rd) { m_renderables.push_back(rd); }
			void detach(const renderable* rd) { m_renderables.remove(rd); }
			void tick();
	};
}

#endif