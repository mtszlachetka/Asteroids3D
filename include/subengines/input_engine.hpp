#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include <GLFW/glfw3.h>
#include <list>
#include <tuple>


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
		q_pressed,
		e_pressed,
		space_pressed,
		left_shift_pressed,
		left_shift_released,
		left_mouse_button_pressed,
		mouse_moved,
		left_ctlr_pressed,
		left_alt_pressed
	};

	
	// every class that reacts to input inherits from this
	class input_listener {
		public:
			input_listener();
			virtual void update(input_event event) = 0;
			virtual ~input_listener();
	};

	class input_engine {
		private:
			int m_window_width, m_window_height;
			double x_cursor_pos, y_cursor_pos;
			double x_offset, y_offset;
			double mouse_sensitivity = 0.003f;
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
			std::tuple<double, double> get_mouse_coords() const {
				return {x_offset, y_offset};
			}
	}; 

}

#endif