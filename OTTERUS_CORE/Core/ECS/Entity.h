#pragma once
#include "Registry.h"

namespace otterus_core::ECS {

	class Entity
	{
	private:
		Registry& m_registry;
		entt::entity m_entity;
		std::string m_name, m_group;

	public:
		Entity(Registry& registry);
		Entity(Registry& registry, const std::string name = "", const std::string group = "");
		~Entity() =  default;

		Entity(Registry& registry, const entt::entity& entity);

		inline const std::string& GetName() const { return m_name; }
		inline const std::string& GetGroup() const { return m_group; }


		inline std::uint32_t Kill() { return m_registry.GetRegistry().destroy(m_entity); };

		inline entt::entity& GetEntity() { return m_entity; }
		inline entt::registry& GetRegistry() { return m_registry.GetRegistry(); }

		template <typename TComponent, typename ...Args>
		TComponent& AddComponent(Args&& ...args);

		template <typename TComponent, typename ...Args>
		TComponent& ReplaceComponent(Args&& ...args);

		template <typename TComponent>
		TComponent& GetComponent();

		template <typename TComponent>
		bool HasComponent();

		template <typename TComponent>
		bool RemoveComponent();
	};

}

#include "Entity.inl"