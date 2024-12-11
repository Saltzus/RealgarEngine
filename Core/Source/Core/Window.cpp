#include "Window.h"

namespace RED
{

	Window::~Window()
	{

	}

	/// @param WinName = Title of the window
	/// @param WinWidth = Width of the window
	/// @param WinHeight = Height of the window
	/// @param Monitor = The monitor to use for full screen mode, or 'NULL' for windowed mode.
	/// @param window = Share The window whose context to share resources with, or 'NULL' to not share resources.
	/// \returns GLFWwindow*
	Window::Window(const char* WinName, int WinWidth, int WinHeight, GLFWmonitor* monitor, GLFWwindow* window)
	{
		width = WinWidth;
		height = WinHeight;

		glfwInit();

		if (Renderer::GetGraphicsApi() == GraphicsApis::Vulkan)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}
		if (Renderer::GetGraphicsApi() == GraphicsApis::OpenGL)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		}

		GLFW_Window = glfwCreateWindow(WinWidth, WinHeight, WinName, monitor, window);
		glfwSetWindowUserPointer(GLFW_Window, this);

		if (GLFW_Window == NULL)
		{
			std::cout << "Failed to create GLFW window  " << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(GLFW_Window);
		glfwSetFramebufferSizeCallback(GLFW_Window, framebuffer_size_callback);

		if (Renderer::GetGraphicsApi() == GraphicsApis::OpenGL)
		{
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
			}
			glEnable(GL_DEPTH_TEST);
		}
	}

	void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (windowInstance)
		{
			windowInstance->onResize(width, height);
		}

		if (Renderer::GetGraphicsApi() == GraphicsApis::OpenGL)
		{
			glViewport(0, 0, width, height);
		}
	}

	void Window::window_size_callback(GLFWwindow* glfwWindow, int width, int height)
	{
		Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
		if (windowInstance)
		{
			windowInstance->onResize(width, height);
		}
	}

	void Window::Display()
	{
		glfwSwapBuffers(GLFW_Window);
		glfwPollEvents();

		if (Renderer::GetGraphicsApi() == GraphicsApis::Vulkan)
		{
		}
		if (Renderer::GetGraphicsApi() == GraphicsApis::OpenGL)
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	void Window::Update()
	{
		glfwSetWindowSizeCallback(this->GLFW_Window, window_size_callback);
		int win_width, win_height;
		glfwGetFramebufferSize(this->GLFW_Window, &win_width, &win_height);
		glViewport(0, 0, win_width, win_height);
	}

	void Window::onResize(int width, int height) {
		this->width = width;
		this->height = height;
		this->resised = true;
	}
}