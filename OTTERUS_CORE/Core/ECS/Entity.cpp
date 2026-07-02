#include "Entity.h"
#include "Components/Identification.h"

namespace otterus_core::ECS {
	Entity::Entity(Registry& registry) :
		Entity(registry, "GameObject", "")
	{}

	Entity::Entity(Registry& registry, const std::string name, const std::string group):
		m_registry{ registry }, m_name{ name }, m_group{ group }
	{
		m_entity = m_registry.GetRegistry().create();

		AddComponent<Identification>(Identification{
			.name = m_name,
			.group = m_group,
			.entity_id = static_cast<int32_t>(m_entity)
			});
	}

}

