#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include <GLFW/glfw3.h>
#include <list>

#include "input_listener.hpp"

namespace se {
   

	class input_engine {
		private:
			GLFWwindow* m_p_active_window = nullptr;
			std::list<input_listener*> m_listeners;
			void notify(input_event e) {
				for (input_listener* l : m_listeners) {
					l->update(e);
				}
			}
			input_engine() {}
		public:
			input_engine(const input_engine& other) = delete;
			input_engine(const input_engine&& other) = delete;
			void operator=(const input_engine& other) = delete;
			void operator=(const input_engine&& other) = delete;
			static input_engine& get_instance() {
				static input_engine instance;
				return instance;
			}
			void tick();
			void attach(input_listener* listener) { m_listeners.push_back(listener); }
			void detach(input_listener* listener) { m_listeners.remove(listener); }
			void set_active_window(GLFWwindow* t_window) { m_p_active_window = t_window; }
	}; 

}

#endif