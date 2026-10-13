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
#include <Rendering/Core/Camera2D.h>
#include <Core/Resources/AssetManager.h>

#include <algorithm>
#include <ImGui.h>

constexpr float one_over_sixty = 1.f / 60.f;

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
		m_SceneLoaded = false;
		m_PlayScene = false;
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

		auto& mainRegistry = MAIN_REGISTRY();
		auto& assetManager = mainRegistry.GetAssetManager();

		auto playTexture = assetManager.GetTexture( "play_button" );
		auto stopTexture = assetManager.GetTexture( "stop_button" );

		if (m_PlayScene)
		{
			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.f, 0.9f, 0.f, 0.3 } );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.9f, 0.f, 0.3 } );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.f, 0.9f, 0.f, 0.3 } );
		}

		if ( ImGui::ImageButton( "##PlayButton",
								 (ImTextureID)playTexture.GetID(),
								 ImVec2{
									 (float)playTexture.GetWidth() * 0.25f,
									 (float)playTexture.GetHeight() * 0.25f,
								 } ) &&
			 !m_SceneLoaded )
		{
			LoadScene();
		}

		if ( ImGui::GetColorStackSize() > 0 )
			ImGui::PopStyleColor( ImGui::GetColorStackSize() );

		if ( ImGui::IsItemHovered( ImGuiHoveredFlags_DelayNormal ) )
			ImGui::SetTooltip( "Play Scene" );

		ImGui::SameLine();

		if ( !m_PlayScene )
		{
			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.f, 0.9f, 0.f, 0.3 } );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.9f, 0.f, 0.3 } );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.f, 0.9f, 0.f, 0.3 } );
		}

		if ( ImGui::ImageButton( "##StopButton",
								 (ImTextureID)stopTexture.GetID(),
								 ImVec2{
									 (float)stopTexture.GetWidth() * 0.25f,
									 (float)stopTexture.GetHeight() * 0.25f,
								 } ) &&
			 m_SceneLoaded )
		{
			UnloadScene();
		}

		if ( ImGui::GetColorStackSize() > 0 )
			ImGui::PopStyleColor( ImGui::GetColorStackSize() );

		if ( ImGui::IsItemHovered( ImGuiHoveredFlags_DelayNormal ) )
			ImGui::SetTooltip( "Stop Scene" );

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

		auto& camera = m_Registry.GetContext<std::shared_ptr<otterus_rendering::Camera2D>>();
		if (!camera) {
			OTTERUS_ERROR("Failed to get Camera2D from Registry.");
			return;
		}
		camera->Update();

		auto& scriptSystem = m_Registry.GetContext<std::shared_ptr<otterus_core::Systems::ScriptingSystem>>();
		scriptSystem->Update();

		if (coreGlobals.IsPhysicsEnabled())
		{
			auto& physicsWorld = m_Registry.GetContext<otterus_physics::PhysicsWorld>();
			physicsWorld->Step(	one_over_sixty, coreGlobals.GetVelocityIterations(), coreGlobals.GetPositionIterations() );
			physicsWorld->ClearForces();
		}

		auto& physicsSystem = m_Registry.GetContext<std::shared_ptr<otterus_core::Systems::PhysicsSystem>>();
		physicsSystem->Update(m_Registry.GetRegistry());

		auto& animationSystem = m_Registry.GetContext<std::shared_ptr<otterus_core::Systems::AnimationSystem>>();
		animationSystem->Update();

        IDisplay::Update();
    }
}
