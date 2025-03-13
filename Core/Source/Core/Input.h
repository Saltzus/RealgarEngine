#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <lua.hpp>

namespace Realgar
{
	class Input
	{
	public:
		static bool KeyDown(const char* key);
		static bool KeyPressed(const char* key);
		static bool KeyReleased(const char* key);

		static bool MouseButtonDown(const char* button);
		static bool MouseButtonPressed(const char* button);
		static bool MouseButtonReleased(const char* button);

		static void registerInput(lua_State* L);

		static void SetWindow(GLFWwindow* window) { Input::window = window; }
	private:
		static int getGLFWMouseButton(std::string key);
		static int getGLFWKey(std::string key);

		static GLFWwindow* window;
	};


}

