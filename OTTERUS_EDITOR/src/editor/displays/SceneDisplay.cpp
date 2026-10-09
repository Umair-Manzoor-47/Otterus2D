#include "SceneDisplay.h"
#include <Rendering/Buffers/Framebuffer.h>
#include <Core/ECS/MainRegistry.h>
#include <Core/Systems/ScriptingSystem.h>
#include <Logger/Logger.h>
#include <Core/Systems/AnimationSystem.h>
#include <Core/Systems/PhysicsSystem.h>
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFxPlayer.h>
#include <Physics/Box2Dwrappers.h>
#include <Core/CoreUtilities/CoreEngineData.h>

#include <algorithm>
#include <ImGui.h>

namespace otterus_editor {

	SceneDisplay::SceneDisplay(otterus_core::ECS::Registry& registry)
		: m_Registry{ registry }
        , m_PlayScene{ false }
        , m_SceneLoaded{ false }
	{}

    void SceneDisplay::LoadScene()
    {

	    auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();

		if (!lua)
			lua = std::make_shared<sol::state>();

	    lua->open_libraries(
	        sol::lib::base,
	        sol::lib::math,
	        sol::lib::os,
	        sol::lib::table,
	        sol::lib::io,
	        sol::lib::string,
	        sol::lib::package
	    );

	    auto& scriptingSystem = m_Registry.GetContext<std::shared_ptr<otterus_core::Systems::ScriptingSystem>>();
	    otterus_core::Systems::ScriptingSystem::RegisterLuaBindings(*lua, m_Registry);
	    otterus_core::Systems::ScriptingSystem::RegisterLuaFunctions(*lua, m_Registry);

	    if (!scriptingSystem->LoadMainScript(*lua)) {

	        OTTERUS_ERROR("Failed to load the main lua script.");
	        return;
	    }
		m_SceneLoaded = true;
		m_PlayScene = true;
    }

    void SceneDisplay::UnloadScene()
    {
		m_SceneLoaded = true;
		m_PlayScene = true;
		m_Registry.GetRegistry().clear();
	    auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();

		lua.reset();

		auto& mainRegistry = MAIN_REGISTRY();
		mainRegistry.GetMusicPlayer().Stop();
		mainRegistry.GetSoundPlayer().Stop(-1);

    }

    void SceneDisplay::Draw()
    {
        static bool isOpen{ true };
        if (!ImGui::Begin("Scene", &isOpen))
        {
            ImGui::End();
            return;
        }

        if (ImGui::BeginChild("##SceneChild", ImVec2{ 0.f, 0.f }, false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
        {
            const auto& fb = m_Registry.GetContext<std::shared_ptr<otterus_rendering::Framebuffer>>();
            const auto& engineData = otterus_core::CoreEngineData::GetInstance();

            int targetWidth  = engineData.WindowWidth();
            int targetHeight = engineData.WindowHeight();

            // Sync framebuffer resolution with target engine design resolution
            if (targetWidth > 0 && targetHeight > 0)
            {
                if (fb->GetWidth() != targetWidth || fb->GetHeight() != targetHeight)
                {
                    fb->Resize(targetWidth, targetHeight);
                }
            }

            // Aspect-fit within available ImGui window
            ImVec2 avail = ImGui::GetContentRegionAvail();
            if (avail.x > 0.0f && avail.y > 0.0f && targetWidth > 0 && targetHeight > 0)
            {
                float scale = std::min(avail.x / targetWidth, avail.y / targetHeight);
                ImVec2 renderSize{ targetWidth * scale, targetHeight * scale };

                ImGui::SetCursorPos(ImVec2{ (avail.x - renderSize.x) * 0.5f, (avail.y - renderSize.y) * 0.5f });

                ImGui::Image(
                    (ImTextureID)fb->GetTextureID(),
                    renderSize,
                    ImVec2{ 0.f, 1.f },
                    ImVec2{ 1.f, 0.f }
                );
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void SceneDisplay::Update()
    {
		if (!m_PlayScene) return;

		auto& mainRegistry = MAIN_REGISTRY();
		auto& coreGlobals = CORE_GLOBALS();

        IDisplay::Update();
    }
}
