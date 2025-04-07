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
			bool editorMode = false,
			GLFWmonitor* monitor = NULL, 
			GLFWwindow* window = NULL
		);

		~Window();

		void Display();
		void Update();

		int width = 0;
		int height = 0;
		bool resised = false;

		static bool editor;
		static int viewportWidth, viewportHeight;

		operator GLFWwindow* () const 
		{
			return GLFW_Window;
		}

	private:
		void toggleFullscreen(GLFWwindow* window);

		bool isFullscreen = false;
		int windowedX, windowedY, windowedWidth, windowedHeight;

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void window_size_callback(GLFWwindow* glfwWindow, int width, int height);
		void onResize(int width, int height);
	};
}