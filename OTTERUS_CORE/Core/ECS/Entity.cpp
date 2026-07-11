#include "Entity.h"
#include "Components/Identification.h"
#include "MetaUtilities.h"

using namespace otterus_core::Utils;
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

	Entity::Entity(Registry& registry, const entt::entity& entity):
		m_registry{ registry }, m_entity{ entity }, m_name{ "" }, m_group{ "" }
	{
		if (m_registry.GetRegistry().all_of<Identification>(m_entity)) {
		
			auto id = GetComponent<Identification>();
			m_name = id.name;
			m_group = id.group;
		}
	
	}

	void Entity::CreateLuaEntityBind(sol::state& lua, Registry& registry)
	{
	
		using namespace entt::literals;
		lua.new_usertype<Entity>(
			"Entity",
			sol::call_constructor,
			sol::factories([&](const std::string& name, const std::string& group) {
					return Entity{registry, name, group};
				}
			),
			"add_component", [&](Entity& entity, const sol::table& comp, sol::this_state s) -> sol::object {
				if (!comp.valid()) 
					return sol::lua_nil_t{};
				
				const auto component = InvokeMetaFunction(
					GetIdType(comp),
					"add_component"_hs,
					entity, comp, s
				);

				return component ? component.cast<sol::reference>() : sol::lua_nil_t{};
			}
		);
	}

}

