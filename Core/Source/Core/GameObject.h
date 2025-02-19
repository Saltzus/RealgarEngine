#pragma once
#include "components/components.h"

#include <unordered_map>
#include <typeindex>

namespace RED
{
	class GameObject
	{
	public:
		std::unordered_map<std::type_index, Components::Component*> components = {};
		std::string name;

		GameObject();
		~GameObject();

		static void registerGameObject(lua_State* L);

		template <typename T>
		T* getComponent()
		{
			auto component = components.find(std::type_index(typeid(T)));
			if (component != components.end())
				return static_cast<T*>(component->second);
			return nullptr;
		}

		template <typename T, typename... Args>
		void addComponent(Args&&... args)
		{
			components[std::type_index(typeid(T))] = new T(std::forward<Args>(args)...);
		}

		void update(float deltaTime);
		void render(Shader* shader, Camera* camera);
	private:

	};
} 
