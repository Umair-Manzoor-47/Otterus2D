#include "TilesetDisplay.h"
#include <Core/Resources/AssetManager.h>
#include <Core/ECS/MainRegistry.h>
#include <Logger/Logger.h>
#include <OtterusUtilities/OtterusUtilities.h>

#include <imgui.h>

namespace otterus_editor {
    void TilesetDisplay::Draw() {

        if (!ImGui::Begin("Tileset")) {
            ImGui::End();
            return;
        }

        auto& mainRegistry = MAIN_REGISTRY();
        auto& assetManager = mainRegistry.GetAssetManager();

        auto tilesets =  otterus_utils::GetKeys(assetManager.GetAllTextures(),
            [](const auto& pair){return pair.second->IsTileset(); }
        );

        if (ImGui::BeginCombo("Choose Tileset", m_Tileset.c_str())) {
            for (const auto& tileset : tilesets) {
                bool isSelected = m_Tileset == tileset;
                if (ImGui::Selectable(tileset.c_str(), isSelected))
                    m_Tileset = tileset;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();

            }

            ImGui::EndCombo();
        }
        if (m_Tileset.empty()) {
            ImGui::End();
            return;
        }

        const auto& texture = assetManager.GetTexture(m_Tileset);
        if (!texture) {
            ImGui::End();
            return;
        }

        int textureWidth = texture.GetWidth();
        int textureHeight = texture.GetHeight();

        constexpr int tileSize = 16; // Change to 32 if your tiles are 32x32

        int cols = textureWidth / tileSize;
        int rows = textureHeight / tileSize;

        if (cols <= 0 || rows <= 0) {
            ImGui::End();
            return;
        }

        ImGuiTableFlags tableFlags{ 0 };
        tableFlags |= ImGuiTableFlags_SizingFixedFit;
        tableFlags |= ImGuiTableFlags_ScrollX;

        int k{ 0 }, id{ 0 };

        if (ImGui::BeginTable("Tileset", cols, tableFlags)) {
            for (int i = 0; i < rows; i++) {
                ImGui::TableNextRow();

                for (int j = 0; j < cols; j++) {
                    ImGui::TableSetColumnIndex(j);

                    if (m_Selected == id) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(
                            ImVec4{ 0.f, 0.9f, 0.f, 0.3f }));
                    }

                    // Normalized UVs [0.0, 1.0] for this specific tile
                    float u0 = static_cast<float>(j * tileSize) / static_cast<float>(textureWidth);
                    float v0 = static_cast<float>(i * tileSize) / static_cast<float>(textureHeight);
                    float u1 = static_cast<float>((j + 1) * tileSize) / static_cast<float>(textureWidth);
                    float v1 = static_cast<float>((i + 1) * tileSize) / static_cast<float>(textureHeight);

                    ImGui::PushID(k++);

                    if (ImGui::ImageButton("##tile", (ImTextureID)texture.GetID(),
                        ImVec2{ tileSize * 1.5f, tileSize * 1.5f },
                        ImVec2{ u0, v0 }, ImVec2{ u1, v1 }))
                    {
                        m_Selected = id;
                        OTTERUS_LOG("startX: {}, startY: {}", j, i);
                    }

                    ImGui::PopID();

                    ++id;
                }
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }
}
