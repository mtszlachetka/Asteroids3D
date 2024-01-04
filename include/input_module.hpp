#ifndef INPUT_MODULE_HPP
#define INPUT_MODULE_HPP

#include <GLFW/glfw3.h>
#include <list>

namespace se {
    
	// possible events
	enum class input_event {
		w_pressed,
		s_pressed,
		a_pressed,
		d_pressed,
		z_pressed,
		c_pressed,
		f_pressed,
		r_pressed,
		x_pressed,
		space_pressed
	};

	// every class taht reacts to input inherits from this
	class input_listener {
		public:
			virtual void update(input_event event) = 0;
			virtual ~input_listener() {}
	};

	class input_module {
		private:
			GLFWwindow* m_p_active_window = nullptr;
			std::list<input_listener*> m_listeners;
			void notify(input_event e) {
				for (input_listener* l : m_listeners) {
					l->update(e);
				}
			}
		public:
			void tick();
			void attach(input_listener* listener) { m_listeners.push_back(listener); }
			void detach(input_listener* listener) { m_listeners.remove(listener); }
			void set_active_window(GLFWwindow* t_window) { m_p_active_window = t_window; }
	}; 

}



#endif