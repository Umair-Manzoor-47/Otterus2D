#include "TilesetDisplay.h"
#include <Core/Resources/AssetManager.h>
#include <Core/ECS/MainRegistry.h>
#include <Logger/Logger.h>

#include <imgui.h>

namespace otterus_editor {
    void TilesetDisplay::Draw() {

        if (!ImGui::Begin("Tileset")) {
            ImGui::End();
            return;
        }

        auto& mainRegistry = MAIN_REGISTRY();
        auto& assetManager = mainRegistry.GetAssetManager();

        ImGui::End();
    }
}
