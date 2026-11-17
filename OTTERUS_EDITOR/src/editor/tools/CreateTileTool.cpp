#include "CreateTileTool.h"
#include "Core/Resources/AssetManager.h"
#include "Core/ECS/MainRegistry.h"
#include "Rendering/Core/Camera2D.h"
#include "Logger/Logger.h"
#include "editor/utilities/EditorUtilities.h"

namespace otterus_editor{
    CreateTileTool::CreateTileTool()
    : TileTool()
    {}

    void CreateTileTool::Create() {
        // TODO: Check out of bounds, active, etc

        // TODO: Check for mouse left button pressed.

        // Create tile entity based of the mouse tile
    }

    void CreateTileTool::Draw() {
        if ( !SpriteValid() || !IsActivated() || OutOfBounds() || !m_Camera  )
            return;

        auto shader = MAIN_REGISTRY().GetAssetManager().GetShader( "basic" );
        if ( !shader )
            return;

        shader->Enable();
        auto camMat = m_Camera->GetCameraMatrix();
        shader->SetUniformMat4( "uProjection", camMat);
        DrawMouseSprite();
        shader->Disable();
    }
}
