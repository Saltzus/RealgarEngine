#pragma once
#include "Shader.h"
#include "Camera.h"

namespace RED::Components
{
	class Component
	{
	public:
		~Component() {}

		virtual void update(float deltaTime) {}
		virtual void execute() {}
		virtual void render(Shader* shader, Camera* camera) {}
	private:

	};
}