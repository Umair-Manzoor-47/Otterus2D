#include "ScriptingSystem.h"
#include "../ECS/Components/ScriptComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/SpriteComponent.h"
#include "../ECS/Entity.h"
#include <logger/Logger.h>

using namespace otterus_core::ECS;

namespace otterus_core::Systems {
	ScriptingSystem::ScriptingSystem(otterus_core::ECS::Registry& registry)
		: m_registry(registry), m_mainLoaded{false}
	{}
	bool ScriptingSystem::LoadMainScript(sol::state & lua)
	{
		auto path = "./assets/scripts/main.lua";

		try {
		
			auto result = lua.safe_script_file(path);
		}
		catch (sol::error& err)
		{
			OTTERUS_ERROR("Failed to load main lua script at [{0}], Error: {1}", path, err.what());
			return false;
		}

		sol::table main_lua = lua["main"];
		sol::optional<sol::table> updateExists = main_lua[1];

		if (updateExists == sol::nullopt)
		{
			OTTERUS_ERROR("There is no update function in main.lua.");
			return false;
		}

		sol::table update_script = main_lua[1];
		sol::function update = update_script["update"];


		sol::optional<sol::table> renderExists = main_lua[2];

		if (renderExists == sol::nullopt)
		{
			OTTERUS_ERROR("There is no render function in main.lua.");
			return false;
		}

		sol::table render_script = main_lua[2];
		sol::function render = render_script["render"];

		otterus_core::ECS::Entity mainLuaScript{m_registry, "main_script", ""};
		mainLuaScript.AddComponent<otterus_core::ECS::ScriptComponent>(
			otterus_core::ECS::ScriptComponent{
			.update = update,
			.render = render
			}
		);

		m_mainLoaded = true;

		return true;
	}
	void ScriptingSystem::Update()
	{
		if (!m_mainLoaded) {
			OTTERUS_ERROR("Main script has not been loaded.");
			return;
		}

		auto view = m_registry.GetRegistry().view<otterus_core::ECS::ScriptComponent>();

		for (const auto& entity : view) {
		
			otterus_core::ECS::Entity ent{ m_registry, entity };
			if (ent.GetName() != "main_script") continue;

			auto& script = ent.GetComponent<otterus_core::ECS::ScriptComponent>();
			auto err = script.update(entity);
			if (!err.valid()) {
				sol::error error = err;
				OTTERUS_ERROR("Error Running the update script: {0}", error.what());
				
			}
		
		}
	
	}
	void ScriptingSystem::Render()
	{
		if (!m_mainLoaded) {
			OTTERUS_ERROR("Main script has not been loaded.");
			return;
		}
		auto view = m_registry.GetRegistry().view<otterus_core::ECS::ScriptComponent>();

		for (const auto& entity : view) {

			otterus_core::ECS::Entity ent{ m_registry, entity };
			if (ent.GetName() != "main_script") continue;

			auto& script = ent.GetComponent<otterus_core::ECS::ScriptComponent>();
			auto err = script.render(entity);
			if (!err.valid()) {
				sol::error error = err;
				OTTERUS_ERROR("Error Running the render script: {0}", error.what());

			}

		}
	}
	void ScriptingSystem::RegisterLuaBindings(sol::state& lua, otterus_core::ECS::Registry& registry)
	{
		Registry::CreateLuaRegistryBind(lua, registry);
		Entity::CreateLuaEntityBind(lua, registry);
		TransformComponent::CreateLuaTransformBind(lua);
		SpriteComponent::CreateStaticLuaBind(lua, registry);

		Entity::RegisterMetaComponent<TransformComponent>();
		Entity::RegisterMetaComponent<SpriteComponent>();
	
		Registry::RegisterMetaComponent<TransformComponent>();
		Registry::RegisterMetaComponent<SpriteComponent>();
	}
}
