#pragma once
#include "../Shader.h"
#include "../Camera.h"

#include <lua.hpp>

namespace Realgar::Components
{
	class Component
	{
	public:
		~Component() {}

		virtual void update(float deltaTime) {}
		virtual void init() {}
		virtual void render(Shader* shader, Camera* camera) {}
	private:

	};
}