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

#include "core/Systems/RenderShapeSystem.h"
#include "Core/Systems/RenderSystem.h"
#include "core/Systems/RenderUISystem.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "editor/utilities/EditorFrameBuffer.h"
#include "physics/ContactListener.h"
#include "Rendering/Core/Renderer.h"

constexpr float one_over_sixty = 1.f / 60.f;
using namespace otterus_rendering;
using namespace otterus_core::Systems;

namespace otterus_editor {

	SceneDisplay::SceneDisplay()
		: m_PlayScene{ false }
        , m_SceneLoaded{ false }
	{}

    void SceneDisplay::LoadScene()
    {
		auto currentScene = SCENE_MANAGER().GetCurrentScene();

		if (!currentScene)
			return;

		auto& runtimeRegistry = currentScene->GetRuntimeRegistry();
	    auto lua = runtimeRegistry.AddToContext<std::shared_ptr<sol::state>>(std::make_shared<sol::state>());

		const auto& canvas = currentScene->GetCanvas();

		runtimeRegistry.AddToContext<std::shared_ptr<Camera2D>>(std::make_shared<Camera2D>(canvas.width, canvas.height));

		auto physicsWorld =
			runtimeRegistry.AddToContext<otterus_physics::PhysicsWorld>(std::make_shared<b2World>( b2Vec2{0.f, 9.8f} ));

		auto contactListener =
			runtimeRegistry.AddToContext<std::shared_ptr<otterus_physics::ContactListener>>(std::make_shared<otterus_physics::ContactListener>());

		physicsWorld->SetContactListener(contactListener.get());

		// Systems
		auto scriptingSystem =
			runtimeRegistry.AddToContext<std::shared_ptr<ScriptingSystem>>(std::make_shared<ScriptingSystem>(runtimeRegistry));

		runtimeRegistry.AddToContext<std::shared_ptr<AnimationSystem>>(
			std::make_shared<AnimationSystem>(runtimeRegistry));

		runtimeRegistry.AddToContext<std::shared_ptr<PhysicsSystem>>(
			std::make_shared<PhysicsSystem>(runtimeRegistry));



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

	    ScriptingSystem::RegisterLuaBindings(*lua, runtimeRegistry);
	    ScriptingSystem::RegisterLuaFunctions(*lua, runtimeRegistry);

	    if (!scriptingSystem->LoadMainScript(*lua)) {

	        OTTERUS_ERROR("Failed to load the main lua script.");
	        return;
	    }
		m_SceneLoaded = true;
		m_PlayScene = true;
    }

	void SceneDisplay::UnloadScene()
	{
		auto currentScene = SCENE_MANAGER().GetCurrentScene();
		auto& runtimeRegistry = currentScene->GetRuntimeRegistry();
		m_SceneLoaded = false;
		m_PlayScene = false;
		runtimeRegistry.ClearRegistry();
		runtimeRegistry.RemoveContext<std::shared_ptr<Camera2D>>();
		runtimeRegistry.RemoveContext<std::shared_ptr<sol::state>>();
		runtimeRegistry.RemoveContext<std::shared_ptr<otterus_physics::PhysicsWorld>>();
		runtimeRegistry.RemoveContext<std::shared_ptr<otterus_physics::ContactListener>>();
		runtimeRegistry.RemoveContext<std::shared_ptr<AnimationSystem>>();
		runtimeRegistry.RemoveContext<std::shared_ptr<PhysicsSystem>>();

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
								 (ImTextureID)playTexture->GetID(),
								 ImVec2{
									 (float)playTexture->GetWidth() * 0.25f,
									 (float)playTexture->GetHeight() * 0.25f,
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

		RenderScene();

		if ( ImGui::ImageButton( "##StopButton",
								 (ImTextureID)stopTexture->GetID(),
								 ImVec2{
									 (float)stopTexture->GetWidth() * 0.25f,
									 (float)stopTexture->GetHeight() * 0.25f,
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
        	auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
        	const auto& fb = editorFramebuffers->mapFramebuffers[ FramebufferType::SCENE ];
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

    void SceneDisplay::RenderScene() {
		auto& mainRegistry = MAIN_REGISTRY();
		auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
		auto& renderer = mainRegistry.GetContext<std::shared_ptr<otterus_rendering::Renderer>>();

		auto& renderSystem = mainRegistry.GetContext<std::shared_ptr<otterus_core::Systems::RenderSystem>>();
		auto& renderUISystem = mainRegistry.GetContext<std::shared_ptr<otterus_core::Systems::RenderUISystem>>();
		auto& renderShapeSystem = mainRegistry.GetContext<std::shared_ptr<otterus_core::Systems::RenderShapeSystem>>();

		const auto& fb = editorFramebuffers->mapFramebuffers[ FramebufferType::SCENE ];

		fb->CheckResize();
		fb->Bind();
		renderer->SetViewport( 0, 0, fb->GetWidth(), fb->GetHeight() );
		renderer->SetClearColor( 0.f, 0.f, 0.f, 1.f );
		renderer->ClearBuffers( true, true, false );

		auto currentScene = SCENE_MANAGER().GetCurrentScene();

		if (currentScene && m_PlayScene)
		{
			auto& runtimeRegistry = currentScene->GetRuntimeRegistry();
			renderSystem->Update(runtimeRegistry);
			renderShapeSystem->Update(runtimeRegistry);
			renderUISystem->Update( runtimeRegistry );
		}

		fb->Unbind();
    }

    void SceneDisplay::Update()
    {
		if (!m_PlayScene) return;

		auto currentScene = SCENE_MANAGER().GetCurrentScene();

		if (!currentScene)
			return;

		auto& runtimeRegistry = currentScene->GetRuntimeRegistry();

		auto& coreGlobals = CORE_GLOBALS();

		auto& camera = runtimeRegistry.GetContext<std::shared_ptr<otterus_rendering::Camera2D>>();
		if (!camera) {
			OTTERUS_ERROR("Failed to get Camera2D from Registry.");
			return;
		}
		camera->Update();

		auto& scriptSystem = runtimeRegistry.GetContext<std::shared_ptr<otterus_core::Systems::ScriptingSystem>>();
		scriptSystem->Update();

		if (coreGlobals.IsPhysicsEnabled())
		{
			auto& physicsWorld = runtimeRegistry.GetContext<otterus_physics::PhysicsWorld>();
			physicsWorld->Step(	one_over_sixty, coreGlobals.GetVelocityIterations(), coreGlobals.GetPositionIterations() );
			physicsWorld->ClearForces();
		}

		auto& physicsSystem = runtimeRegistry.GetContext<std::shared_ptr<otterus_core::Systems::PhysicsSystem>>();
		physicsSystem->Update(runtimeRegistry.GetRegistry());

		auto& animationSystem = runtimeRegistry.GetContext<std::shared_ptr<otterus_core::Systems::AnimationSystem>>();
		animationSystem->Update();

        IDisplay::Update();
    }
}
