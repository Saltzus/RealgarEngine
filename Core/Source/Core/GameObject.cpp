#include "GameObject.h"

namespace RED
{
	GameObject::GameObject()
	{
	}

	GameObject::~GameObject()
	{
		for (auto& comp : components)
		{
			components.erase(comp.first);
			delete comp.second;
		}
	}


	void GameObject::update(float deltaTime) {
		for (auto comp : components)
			comp.second->update(deltaTime);
	}

	void GameObject::render(Shader* shader, Camera* camera)
	{
		Components::RenderComponent* renderComponent = getComponent<Components::RenderComponent>();
		if (renderComponent != nullptr)
			renderComponent->render(shader, camera);
	}
}