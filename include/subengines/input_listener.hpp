#ifndef INPUT_LISTENER_HPP
#define INPUT_LISTENER_HPP

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
} // namespace se 

#endif