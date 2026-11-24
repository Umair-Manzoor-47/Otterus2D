#include "TilemapDisplay.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/Resources/AssetManager.h"
#include "Rendering/Core/Camera2D.h"
#include "Rendering/Core/Renderer.h"
#include "Core/Systems/RenderSystem.h"
#include "Core/Systems/RenderUISystem.h"
#include "Core/Systems/RenderShapeSystem.h"

#include "editor/systems/GridSystem.h"
#include "editor/utilities/EditorFrameBuffer.h"

#include "Logger/Logger.h"
#include <imgui.h>

#include "Core/Scripting/InputManager.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "editor/utilities/EditorUtilities.h"
#include "editor/tools/ToolManager.h"
#include "editor/tools/CreateTileTool.h"

using namespace otterus_core::Systems;
namespace otterus_editor
{
void TilemapDisplay::RenderTilemap()
{
	auto currentScene = SCENE_MANAGER().GetCurrentScene();
	auto& mainRegistry = MAIN_REGISTRY();
	auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
	auto& renderer = mainRegistry.GetContext<std::shared_ptr<otterus_rendering::Renderer>>();

	auto& renderSystem = mainRegistry.GetContext<std::shared_ptr<RenderSystem>>();
	auto& renderUISystem = mainRegistry.GetContext<std::shared_ptr<RenderUISystem>>();
	auto& renderShapeSystem = mainRegistry.GetContext<std::shared_ptr<RenderShapeSystem>>();

	const auto& fb = editorFramebuffers->mapFramebuffers[ FramebufferType::TILEMAP ];

	fb->CheckResize();
	fb->Bind();
	renderer->SetViewport( 0, 0, fb->GetWidth(), fb->GetHeight() );
	renderer->SetClearColor( 0.1f, 0.1f, 0.1f, 1.f ); // or 0.f, 0.f, 0.f, 1.f
	renderer->ClearBuffers( true, true, false );
	if (!currentScene)
	{
		fb->Unbind();
		return;
	}

	auto& gridSystem = mainRegistry.GetContext<std::shared_ptr<GridSystem>>();
	gridSystem->Update( *currentScene, *m_pTilemapCam );

	renderSystem->Update(currentScene->GetRegistry(), *m_pTilemapCam, currentScene->GetLayerParams());
	renderShapeSystem->Update(currentScene->GetRegistry(), *m_pTilemapCam);
	renderUISystem->Update( currentScene->GetRegistry());

	auto activeTool = SCENE_MANAGER().GetToolManager().GetActiveTool();
	if (activeTool)
	{
		activeTool->Draw();
	}

	fb->Unbind();

}

void TilemapDisplay::LoadNewScene()
{
	auto currentScene = SCENE_MANAGER().GetCurrentScene();
	if (!currentScene)
		return;

	auto activeTool = SCENE_MANAGER().GetToolManager().GetActiveTool();

	if (activeTool)
	{
		if (!activeTool->SetupTool(currentScene->GetRegistryPtr(), m_pTilemapCam.get()))
		{
			OTTERUS_ASSERT(false && "Tool setup failed.");
			__debugbreak();
		}

		if (!SCENE_MANAGER().GetCurrentTileset().empty())
		{
			activeTool->LoadSpriteTextureData(SCENE_MANAGER().GetCurrentTileset());
		}
	}
}

void TilemapDisplay::PanZoomCamera(const glm::vec2 &mousePos) {
	if (!m_pTilemapCam)
		return;

	auto& mouse = INPUT_MANAGER().GetMouse();

	if ( !mouse.IsBtnJustPressed(OT_MOUSE_MIDDLE) && !mouse.IsBtnPressed(OT_MOUSE_MIDDLE)
		&& mouse.GetMouseWheelY() == 0 )
		return;

	static glm::vec2 startPos{0.f};
	auto screenOffset = m_pTilemapCam->GetScreenOffset();
	bool offsetChanged{ false }, scaleChanged{ false };

	if (mouse.IsBtnJustPressed(OT_MOUSE_MIDDLE)) {
		startPos = mousePos;
	}

	if (mouse.IsBtnPressed(OT_MOUSE_MIDDLE)) {
		screenOffset += (mousePos - startPos);
		offsetChanged = true;
	}

	glm::vec2 currentWorldPos = m_pTilemapCam->ScreenCoordToWorld( mousePos );
	float scale = m_pTilemapCam->GetScale();

	if (mouse.GetMouseWheelY() == 1) {

		scale += 0.2f;
		scaleChanged = true;
		offsetChanged = true;

	} else if (mouse.GetMouseWheelY() == -1) {

		scale -= 0.2f;
		scaleChanged = true;
		offsetChanged = true;
	}

	scale = std::clamp(scale, 1.f, 10.f);

	if (scaleChanged) {
		m_pTilemapCam->SetScale(scale);
	}

	glm::vec2 afterMovePos = m_pTilemapCam->ScreenCoordToWorld(mousePos);
	screenOffset += (afterMovePos - currentWorldPos);

	if (offsetChanged) {
		m_pTilemapCam->SetScreenOffset( screenOffset );
	}
	startPos = mousePos;
}

TilemapDisplay::TilemapDisplay()
	: m_pTilemapCam{ std::make_unique<otterus_rendering::Camera2D>() }
{}

void TilemapDisplay::Draw()
{
	if ( !ImGui::Begin( "Tilemap Editor" ) )
	{
		ImGui::End();
		return;
	}

	RenderTilemap();

	auto& mainRegistry = MAIN_REGISTRY();

	if ( ImGui::BeginChild( "##tilemap", ImVec2{ 0, 0 }, false, ImGuiWindowFlags_NoScrollWithMouse ) )
	{
		auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
		const auto& fb = editorFramebuffers->mapFramebuffers[ FramebufferType::TILEMAP ];

		ImVec2 imageSize{ static_cast<float>( fb->GetWidth() ), static_cast<float>( fb->GetHeight() ) };
		ImVec2 windowSize{ ImGui::GetWindowSize() };

		float x = ( windowSize.x - imageSize.x ) * 0.5f;
		float y = ( windowSize.y - imageSize.y ) * 0.5f;

		ImGui::SetCursorPos( ImVec2{ x, y } );
		ImGuiIO io = ImGui::GetIO();
		auto relativePos = ImGui::GetCursorScreenPos();
		auto windowPos = ImGui::GetWindowPos();

		if (auto activeTool = SCENE_MANAGER().GetToolManager().GetActiveTool())
		{
			activeTool->SetRelativeCoords( glm::vec2{ relativePos.x, relativePos.y } );
			activeTool->SetCursorCoords( glm::vec2{ io.MousePos.x, io.MousePos.y } );
			activeTool->SetWindowPos( glm::vec2{ windowPos.x, windowPos.y } );
			activeTool->SetWindowSize( glm::vec2{ windowSize.x, windowSize.y } );

			activeTool->SetOverTilemapWindow(ImGui::IsWindowHovered());
		}
		ImGui::Image( (ImTextureID)fb->GetTextureID(), imageSize, ImVec2{ 0.f, 1.f }, ImVec2{ 1.f, 0.f } );

		// Accept incoming Scene Drop
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_SCENE_SRC);
			if (payload)
			{
				SCENE_MANAGER().SetCurrentScene( std::string{ (const char*)payload->Data } );
				LoadNewScene();
				m_pTilemapCam->Reset();
			}
			ImGui::EndDragDropTarget();
		}

		if ( fb->GetWidth() != static_cast<int>( windowSize.x ) || fb->GetHeight() != static_cast<int>( windowSize.y ) )
		{
			fb->Resize( static_cast<int>( windowSize.x ), static_cast<int>( windowSize.y ) );
			m_pTilemapCam->Resize(static_cast<int>( windowSize.x ), static_cast<int>( windowSize.y ));
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void TilemapDisplay::Update()
{
	auto currentScene = SCENE_MANAGER().GetCurrentScene();
	if (!currentScene)
		return;

	auto activeTool = SCENE_MANAGER().GetToolManager().GetActiveTool();
	if (activeTool && activeTool->IsOverTilemapWindow() && !ImGui::GetDragDropPayload())
	{
		PanZoomCamera(activeTool->GetMouseScreenCoords());

		activeTool->Update(currentScene->GetCanvas());
		activeTool->Create();
	}

	m_pTilemapCam->Update();
}

}
