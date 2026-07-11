#pragma once

#include "Entity.h"

namespace otterus_core::ECS {
	template <typename TComponent, typename ...Args>
	TComponent& Entity::AddComponent(Args&& ...args) {
	
		auto& registry = m_registry.GetRegistry();
		return registry.emplace<TComponent>(m_entity, std::forward<Args>(args) ...);
	
	}

	template <typename TComponent, typename ...Args>
	TComponent& Entity::ReplaceComponent(Args&& ...args) {
		auto& registry = m_registry.GetRegistry();
		if(registry.all_of<TComponent>(m_entity))
			return registry.replace<TComponent>(m_entity, std::forward<Args>(args) ...);
		else
			return registry.emplace<TComponent>(m_entity, std::forward<Args>(args) ...);
	
	}

	template <typename TComponent>
	TComponent& Entity::GetComponent() {
		auto& registry = m_registry.GetRegistry();
		return registry.get<TComponent>(m_entity);

	}

	template <typename TComponent>
	bool Entity::HasComponent() {
		auto& registry = m_registry.GetRegistry();
		return registry.all_of<TComponent>(m_entity);
	
	}

	template <typename TComponent>
	bool Entity::RemoveComponent() {
		auto& registry = m_registry.GetRegistry();
		return registry.remove<TComponent>(m_entity);

	}

	template<typename TComponent>
	auto add_component(Entity& entity, const sol::table& comp, sol::this_state s)
	{
		auto& component = entity.AddComponent(
		 comp.valid ? comp.as<TComponent>() : TComponent{}
		);
		
		return sol::make_reference(s, std::ref(component));
	}

	template<typename TComponent>
	inline void Entity::RegisterMetaComponent()
	{
		using namespace entt::literals;
		entt::meta<TComponent>()
			.type(entt::type_hash<TComponent>::value())
			.template func<&add_component>>("add_component"_hs);

	}

}