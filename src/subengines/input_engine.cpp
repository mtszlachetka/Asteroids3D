#include "subengines/input_engine.hpp"

namespace se
{
	input_listener::input_listener()
	{
		input_engine::get_instance().attach(this);
	}
	input_listener::~input_listener()
	{
		input_engine::get_instance().detach(this);
	}

	void input_engine::tick()
	{
		if (m_p_active_window != nullptr)
		{
			glfwGetWindowSize(m_p_active_window, &m_window_width, &m_window_height);
			glfwGetCursorPos(m_p_active_window, &x_cursor_pos, &y_cursor_pos);
			x_offset = (x_cursor_pos - m_window_width / 2) * mouse_sensitivity;
			y_offset = (m_window_height / 2 - y_cursor_pos) * mouse_sensitivity;

			if (x_offset != 0 || y_offset != 0)
				notify(input_event::mouse_moved);

			if (glfwGetKey(m_p_active_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(m_p_active_window, true);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_W) == GLFW_PRESS)
			{
				notify(input_event::w_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_W) == GLFW_RELEASE)
			{
				notify(input_event::w_released);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_S) == GLFW_PRESS)
			{
				notify(input_event::s_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_A) == GLFW_PRESS)
			{
				notify(input_event::a_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_D) == GLFW_PRESS)
			{
				notify(input_event::d_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_Z) == GLFW_PRESS)
			{
				notify(input_event::z_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_X) == GLFW_PRESS)
			{
				notify(input_event::x_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_R) == GLFW_PRESS)
			{
				notify(input_event::r_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_F) == GLFW_PRESS)
			{
				notify(input_event::f_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				notify(input_event::q_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_E) == GLFW_PRESS)
			{
				notify(input_event::e_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_C) == GLFW_PRESS)
			{
				notify(input_event::c_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				notify(input_event::space_pressed);
			}
			if (glfwGetMouseButton(m_p_active_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				notify(input_event::left_mouse_button_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				notify(input_event::left_shift_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			{
				notify(input_event::left_shift_released);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			{
				notify(input_event::left_alt_pressed);
			}
			if (glfwGetKey(m_p_active_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				notify(input_event::left_ctlr_pressed);
			}
		}
	}
}