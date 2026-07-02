#include "Registry.h"

otterus_core::ECS::Registry::Registry() :
	m_registry{nullptr}
{
	m_registry = std::make_unique<entt::registry>();
}
