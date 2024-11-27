#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace RED 
{
	class Window
	{
	public:
		unsigned int ID;
		GLFWwindow* GLFW_Window;

		Window
		(
			const char* WinName,
			int WinWidth,
			int WinHeight,
			GLFWmonitor* monitor = NULL, 
			GLFWwindow* window = NULL
		);

		~Window();


		void Update();

		int width = 0;
		int height = 0;


		operator GLFWwindow* () const 
		{
			return GLFW_Window;
		}

	private:
		static void window_size_callback(GLFWwindow* glfwWindow, int width, int height);
		void onResize(int width, int height);
	};
}