#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <GLFW/glfw3.h>

namespace se
{
	class game_clock
	{
	private:
		float m_current_frame_time = 0.f;
		float m_previous_frame_time = 0.f;
		float m_delta_time = 0.f;
		float m_initial_time = 0.f;
		game_clock() {}

	public:
		game_clock(const game_clock &other) = delete;
		game_clock(const game_clock &&other) = delete;
		void operator=(const game_clock &other) = delete;
		void operator=(const game_clock &&other) = delete;
		static game_clock &get_instance()
		{
			static game_clock instance;
			return instance;
		}
		void init()
		{
			m_initial_time = static_cast<float>(glfwGetTime());
		}
		float get_current_frame_time() const { return m_current_frame_time; }
		float get_previous_frame_time() const { return m_previous_frame_time; }
		float get_delta_time() const { return m_delta_time; }
		void tick()
		{
			float new_time = glfwGetTime() - m_initial_time;
			m_delta_time = new_time - m_current_frame_time;
			m_previous_frame_time = m_current_frame_time;
			m_current_frame_time = new_time;
		}
	};
}

#endif