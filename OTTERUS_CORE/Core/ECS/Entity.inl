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

}