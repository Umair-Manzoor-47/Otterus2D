#include "SceneDisplay.h"
#include <ImGui.h>
#include <Rendering/Buffers/Framebuffer.h>
#include <Core/CoreUtilities/CoreEngineData.h>
#include <algorithm>

namespace otterus_editor {
	SceneDisplay::SceneDisplay(otterus_core::ECS::Registry& registry)
		: m_Registry{ registry }
	{
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

}