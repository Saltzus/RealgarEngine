#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

#include <iostream>

namespace Realgar 
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

		void Display();
		void Update();

		int width = 0;
		int height = 0;
		bool resised = false;

		operator GLFWwindow* () const 
		{
			return GLFW_Window;
		}

	private:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void window_size_callback(GLFWwindow* glfwWindow, int width, int height);
		void onResize(int width, int height);
	};
}