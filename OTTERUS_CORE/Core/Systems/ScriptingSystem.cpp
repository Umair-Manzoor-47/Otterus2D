#include "ScriptingSystem.h"
#include "../ECS/Components/ScriptComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/SpriteComponent.h"
#include "../ECS/Components/AnimationComponent.h"
#include "../ECS/Components/BoxColliderComponent.h"
#include "../ECS/Components/CircleColliderComponent.h"
#include "../ECS/Components/PhysicsComponent.h"
#include "../ECS/Components/TextComponent.h"
#include "../ECS/Components/RigidBodyComponent.h"

#include "../ECS/Entity.h"
#include <logger/Logger.h>
#include "../Scripting/GlmLuaBindings.h"
#include "../Scripting/InputManager.h"
#include "../Resources/AssetManager.h"
#include "../Scripting/SoundBindings.h"
#include "../Scripting/RendererBindings.h"
#include "../Scripting/UserDataBindings.h"
#include "../Scripting/ContactListenerBind.h"
#include <OtterusUtilities/Timer.h>
#include <OtterusUtilities/RandomGenerator.h>

#include "../CoreUtilities/CoreEngineData.h"
#include "../CoreUtilities/FollowCamera.h"



using namespace otterus_core::ECS;
using namespace otterus_resources;

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

		auto lua = m_registry.GetContext<std::shared_ptr<sol::state>>();
		lua->collect_garbage();
	
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

		auto lua = m_registry.GetContext<std::shared_ptr<sol::state>>();
		lua->collect_garbage();
	}

	auto create_timer = [](sol::state& lua) {
		using namespace otterus_utils;
		lua.new_usertype<Timer>(
			"Timer",
			sol::call_constructor,
			sol::constructors<Timer()>(),
			"start", &Timer::Start,
			"stop", &Timer::Stop,
			"pause", &Timer::Pause,
			"resume", &Timer::Resume,
			"is_running", &Timer::IsRunning,
			"is_paused", &Timer::IsPaused,
			"elapsed_ms", &Timer::ElapsedMS,
			"elapsed_sec", &Timer::ElapsedSec,
			"restart", [](Timer& timer) {
				if (timer.IsRunning())
					timer.Stop();

				timer.Start();
			}
		
		);

		
	};

	auto create_lua_logger = [](sol::state& lua) {
		auto log_handler = [](sol::this_state s, otterus_logger::LogType type, sol::variadic_args va) {
			if (va.size() == 0) return;

			sol::state_view L = s;
			std::string message;

			if (va.size() == 1) {
				sol::object obj = va[0];
				sol::protected_function to_str = L["tostring"];
				auto res = to_str(obj);
				if (res.valid()) {
					message = res.get<std::string>();
				} else {
					message = "<unprintable object>";
				}
			} else if (va[0].is<std::string>()) {
				sol::protected_function str_format = L["string"]["format"];
				auto result = str_format(va);
				if (result.valid()) {
					message = result.get<std::string>();
				} else {
					for (auto it = va.begin(); it != va.end(); ++it) {
						if (!message.empty()) message += "\t";
						sol::object obj = *it;
						sol::protected_function to_str = L["tostring"];
						auto res = to_str(obj);
						if (res.valid()) message += res.get<std::string>();
					}
				}
			} else {
				for (auto it = va.begin(); it != va.end(); ++it) {
					if (!message.empty()) message += "\t";
					sol::object obj = *it;
					sol::protected_function to_str = L["tostring"];
					auto res = to_str(obj);
					if (res.valid()) message += res.get<std::string>();
				}
			}

			auto& logger = otterus_logger::Logger::GetInstance();
			switch (type) {
				case otterus_logger::LogType::INFO:
					logger.LuaLog(message);
					break;
				case otterus_logger::LogType::WARN:
					logger.LuaWarn(message);
					break;
				case otterus_logger::LogType::ERR:
					logger.LuaError(message);
					break;
				default:
					break;
			}
		};

		auto ot_log_fn = [log_handler](sol::this_state s, sol::variadic_args va) {
			log_handler(s, otterus_logger::LogType::INFO, va);
		};
		auto ot_warn_fn = [log_handler](sol::this_state s, sol::variadic_args va) {
			log_handler(s, otterus_logger::LogType::WARN, va);
		};
		auto ot_error_fn = [log_handler](sol::this_state s, sol::variadic_args va) {
			log_handler(s, otterus_logger::LogType::ERR, va);
		};

		// Direct C++ global functions
		lua.set_function("OT_log", ot_log_fn);
		lua.set_function("OT_warn", ot_warn_fn);
		lua.set_function("OT_error", ot_error_fn);

		lua.set_function("OTWarn", ot_warn_fn);
		lua.set_function("OTError", ot_error_fn);

		// OTLog table with __call: supports OTLog("...") and OTLog.log / warn / error
		sol::table otLog = lua.create_named_table("OTLog");
		otLog["log"] = ot_log_fn;
		otLog["warn"] = ot_warn_fn;
		otLog["error"] = ot_error_fn;

		sol::table otLogMeta = lua.create_table();
		otLogMeta[sol::meta_function::call] = [ot_log_fn](sol::table, sol::this_state s, sol::variadic_args va) {
			ot_log_fn(s, va);
		};
		otLog[sol::metatable_key] = otLogMeta;

		// Backward-compatible Logger table: Logger.log, Logger.warn, Logger.error
		sol::table loggerTable = lua.create_named_table("Logger");
		loggerTable["log"] = ot_log_fn;
		loggerTable["warn"] = ot_warn_fn;
		loggerTable["error"] = ot_error_fn;

		// Idiomatic Lua assert hook with error logging and original return values
		lua.safe_script(R"(
				local orig_assert = assert
				assert = function(cond, message, ...)
					if not cond then 
						if select("#", ...) == 0 then
							OT_error(tostring(message or "assertion failed!"))
						else
							OT_error(string.format(message, ...))
						end
					end 
					return orig_assert(cond, message)
				end
			)");
	};

	void ScriptingSystem::RegisterLuaBindings(sol::state& lua, otterus_core::ECS::Registry& registry)
	{
		otterus_core::Scripting::GLMBindings::CreateGLMBindings(lua);
		otterus_core::InputManager::CreateLuaBindings(lua, registry);
		AssetManager::CreateLuaAssetManager(lua, registry);
		otterus_core::Scripting::SoundBinder::CreateSoundBind(lua, registry);
		otterus_core::Scripting::RendererBinder::CreateRendererBind(lua, registry);
		otterus_core::Scripting::UserDataBinder::CreateLuaUserData(lua);
		otterus_core::Scripting::ContactListenerBinder::CreateLuaContactListener(lua, registry.GetRegistry());
		otterus_core::FollowCamera::CreateLuaFollowCamera(lua, registry);

		create_timer(lua);
		create_lua_logger(lua);

		Registry::CreateLuaRegistryBind(lua, registry);
		Entity::CreateLuaEntityBind(lua, registry);
		TransformComponent::CreateLuaTransformBind(lua);
		SpriteComponent::CreateStaticLuaBind(lua, registry);
		AnimationComponent::CreateAnimationLuaBind(lua);
		BoxColliderComponent::CreateBoxColliderLuaBind(lua);
		CircleColliderComponent::CreateLuaCircleColliderBind(lua);
		PhysicsComponent::CreatePhysicsLuaBind(lua, registry.GetRegistry());
		TextComponent::CreateLuaTextBindings(lua);
		RigidBodyComponent::CreateRigidBodyBind(lua);

		Entity::RegisterMetaComponent<TransformComponent>();
		Entity::RegisterMetaComponent<SpriteComponent>();
		Entity::RegisterMetaComponent<AnimationComponent>();
		Entity::RegisterMetaComponent<BoxColliderComponent>();
		Entity::RegisterMetaComponent<CircleColliderComponent>();
		Entity::RegisterMetaComponent<PhysicsComponent>();
		Entity::RegisterMetaComponent<TextComponent>();
		Entity::RegisterMetaComponent<RigidBodyComponent>();

		Registry::RegisterMetaComponent<TransformComponent>();
		Registry::RegisterMetaComponent<SpriteComponent>();
		Registry::RegisterMetaComponent<AnimationComponent>();
		Registry::RegisterMetaComponent<BoxColliderComponent>();
		Registry::RegisterMetaComponent<CircleColliderComponent>();
		Registry::RegisterMetaComponent<PhysicsComponent>();
		Registry::RegisterMetaComponent<TextComponent>();
		Registry::RegisterMetaComponent<RigidBodyComponent>();

		// TODO: Register any required UserData types
		otterus_core::Scripting::UserDataBinder::register_meta_user_data<ObjectData>();
	}
	void ScriptingSystem::RegisterLuaFunctions(sol::state& lua, otterus_core::ECS::Registry& registry)
	{
		lua.set_function(
			"run_script", [&](const std::string& path) {
			
				try {
				
					lua.safe_script_file(path);
				}
				catch (const sol::error& error) {
				
					OTTERUS_ERROR("Error loading lua script file: {}", error.what());
					return false;
				
				}
				return true;
			}
		);
		lua.set_function("get_ticks", [] {
			return SDL_GetTicks();
			}
		);

		auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();
		lua.set_function("measure_text", [&](const std::string& text, const std::string& fontName) {
			const auto& pFont = assetManager->GetFont(fontName);
			if (!pFont)
			{
				OTTERUS_ERROR("Failed to get font [{}] - Does not exist in asset manager!", fontName);
				return -1.f;
			}

			glm::vec2 position{ 0.f }, temp_pos{ position };
			for (const auto& character : text)
				pFont->GetNextCharPos(character, temp_pos);

			return std::abs((position - temp_pos).x);
			}
		);

		auto& engine = CoreEngineData::GetInstance();
		lua.set_function("GetDeltaTime", [&] { return engine.GetDeltaTime(); });
		lua.set_function("WindowWidth", [&] { return engine.WindowWidth(); });
		lua.set_function("WindowHeight", [&] { return engine.WindowHeight(); });

		// Physics Enable functions
		lua.set_function("EnablePhysics", [&] { engine.EnablePhysics(); });
		lua.set_function("DisablePhysics", [&] { engine.DisablePhysics(); });
		lua.set_function("IsPhysicsEnabled", [&] { return engine.IsPhysicsEnabled(); });

		// Render Collider Enable functions
		lua.set_function("EnableRenderColliders", [&] { engine.EnableColliderRender(); });
		lua.set_function("DisableRenderColliders", [&] { engine.DisableColliderRender(); });
		lua.set_function("IsRenderCollidersEnabled", [&] { engine.RenderCollidersEnabled(); });

		lua.new_usertype<otterus_utils::RandomGenerator>(
			"Random",
			sol::call_constructor,
			sol::constructors<otterus_utils::RandomGenerator(uint32_t, uint32_t), otterus_utils::RandomGenerator()>(),
			"get_float", &otterus_utils::RandomGenerator::GetFloat,
			"get_int", &otterus_utils::RandomGenerator::GetInt
		);
	
	}
}
