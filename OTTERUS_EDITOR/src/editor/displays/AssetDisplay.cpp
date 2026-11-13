#include "AssetDisplay.h"
#include "OtterusUtilities/OtterusUtilities.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/Scripting/InputManager.h"
#include "Core/Resources/AssetManager.h"
#include "Logger/Logger.h"
#include <fmt/format.h>


#include <imgui.h>
#include <algorithm>

#include "imgui_internal.h"
#include "editor/scene/SceneManager.h"
#include "editor/utilities/EditorUtilities.h"

constexpr float DEFAULT_ASSET_SIZE = 128.f;
constexpr float MIN_ASSET_SIZE = 32.f;
constexpr float MAX_ASSET_SIZE = 512.f;
constexpr float ASSET_SIZE_STEP = 16.f;
constexpr ImVec2 DRAG_ASSET_SIZE = ImVec2{ 32.f, 32.f };
namespace otterus_editor {
    void AssetDisplay::SetAssetType()
    {
        if ( !m_AssetTypeChanged )
            return;

        if ( m_SelectedAssetType == "TEXTURES" )
        {
            m_SelectedType = otterus_utils::AssetType::TEXTURE;
            m_DragSource = std::string{ DROP_TEXTURE_SRC };
        }
        else if ( m_SelectedAssetType == "FONTS" )
        {
            m_SelectedType = otterus_utils::AssetType::FONT;
            m_DragSource = std::string{ DROP_FONT_SRC };
        }
        else if ( m_SelectedAssetType == "SOUNDFX" )
        {
            m_SelectedType = otterus_utils::AssetType::SOUNDFX;
            m_DragSource = std::string{ DROP_SOUNDFX_SRC };
        }
        else if ( m_SelectedAssetType == "MUSIC" )
        {
            m_SelectedType = otterus_utils::AssetType::MUSIC;
            m_DragSource = std::string{ DROP_MUSIC_SRC };
        }
        else if ( m_SelectedAssetType == "SCENES" )
        {
            m_SelectedType = otterus_utils::AssetType::SCENE;
            m_DragSource = std::string{ DROP_SCENE_SRC };
        }
        else
        {
            m_SelectedType = otterus_utils::AssetType::NO_TYPE;
            m_DragSource = "NO_ASSET_TYPE";
        }

        m_AssetTypeChanged = false;
    }

    void AssetDisplay::DrawSelectedAssets() {
        	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	std::vector<std::string> assetNames{};

	if ( m_SelectedType == otterus_utils::AssetType::SCENE )
	{
		assetNames = SCENE_MANAGER().GetSceneNames();
	}
	else
		assetNames = assetManager.GetAssetKeyNames( m_SelectedType );

	if ( assetNames.empty() )
		return;

	float windowWidth = ImGui::GetWindowWidth();

	int numCols = std::max( 1, static_cast<int>( ( windowWidth - m_AssetSize ) / m_AssetSize ) );
	int numRows = static_cast<int>( assetNames.size() / ( numCols <= 1 ? 1 : numCols ) + 1 );

	if ( !numRows )
		return;

	ImGuiTableFlags tableFlags{ 0 };
	tableFlags |= ImGuiTableFlags_SizingFixedFit;

	int k{ 0 }, id{ 0 };

	auto assetItr = assetNames.begin();

	if ( ImGui::BeginTable( "Assets", numCols, tableFlags ) )
	{
		for ( int row = 0; row < numRows; row++ )
		{
			ImGui::TableNextRow();
			for ( int col = 0; col < numCols; col++ )
			{
				if ( assetItr == assetNames.end() )
					break;

				ImGui::PushID( k++ );
				ImGui::TableSetColumnIndex( col );

				bool bSelectedAsset{ m_SelectedID == id };

				if ( bSelectedAsset )
					ImGui::TableSetBgColor( ImGuiTableBgTarget_CellBg,
											ImGui::GetColorU32( ImVec4{ 0.f, 0.9f, 0.f, 0.3f } ) );

				GLuint textureID{ GetTextureID( *assetItr ) };
				std::string sCheckName{ m_RenameBuf.data() };

				if ( textureID == 0 )
					break;

				ImGui::ImageButton( "##AssetBtn", (ImTextureID)(intptr_t)textureID, ImVec2{ m_AssetSize, m_AssetSize } );

				if ( ImGui::IsItemHovered() && ImGui::IsMouseClicked( 0 ) && !m_Rename )
					m_SelectedID = id;

				auto sAssetName = ( *assetItr ).c_str();

				if ( bSelectedAsset && ImGui::BeginPopupContextItem() )
				{
					if ( ImGui::Selectable( "rename" ) )
					{
						m_Rename = true;
					}
					if ( ImGui::Selectable( "delete" ) )
					{
						if ( !DoDeleteAsset( *assetItr ) )
						{
							OTTERUS_ERROR( "Failed to delete asset [{}]", *assetItr );
						}
						m_SelectedID = -1;
					}
					ImGui::EndPopup();
				}

				if ( ImGui::BeginDragDropSource() )
				{
					ImGui::SetDragDropPayload( m_DragSource.c_str(),
											   sAssetName,
											   ( strlen( sAssetName ) + 1 ) * sizeof( char ),
											   ImGuiCond_Once );

					ImGui::Image( (ImTextureID)textureID, DRAG_ASSET_SIZE );
					ImGui::EndDragDropSource();
				}

				if ( !m_Rename || !bSelectedAsset )
					ImGui::Text( sAssetName );

				if ( m_Rename && bSelectedAsset )
				{
					ImGui::SetKeyboardFocusHere();
					if ( ImGui::InputText(
							 "##rename", m_RenameBuf.data(), 255, ImGuiInputTextFlags_EnterReturnsTrue ) )
					{
						if ( !DoRenameAsset( *assetItr, sCheckName ) )
						{
							OTTERUS_ERROR( "Failed to change asset name." );
						}

						m_RenameBuf.clear();
						m_Rename = false;
					}
					else if ( m_Rename && ImGui::IsKeyPressed( ImGuiKey_Escape ) )
					{
						m_RenameBuf.clear();
						m_Rename = false;
					}
				}

				if ( !m_Rename && bSelectedAsset && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) )
				{
					m_RenameBuf.clear();
					m_RenameBuf = *assetItr;
					m_Rename = true;
				}

				if ( m_Rename && bSelectedAsset )
				{
					if ( sAssetName != sCheckName )
						CheckRename( sCheckName );
				}

				++id;
				++assetItr;
				ImGui::PopID();
			}
		}

		ImGui::EndTable();
	}
    }

    unsigned int AssetDisplay::GetTextureID(const std::string &sAssetName) const {

        auto& assetManager = MAIN_REGISTRY().GetAssetManager();
        switch ( m_SelectedType )
        {
            case otterus_utils::AssetType::TEXTURE: {
                auto texture = assetManager.GetTexture( sAssetName );
                if ( texture )
                    return texture->GetID();

                break;
            }
            case otterus_utils::AssetType::FONT: {
                auto pFont = assetManager.GetFont( sAssetName );
                if ( pFont )
                    return pFont->GetFontAtlasID();

                break;
            }
            case otterus_utils::AssetType::SOUNDFX:
            case otterus_utils::AssetType::MUSIC: {
                auto pTexture = assetManager.GetTexture( "music_icon" );
                if ( pTexture )
                    return pTexture->GetID();

                break;
            }
            case otterus_utils::AssetType::SCENE: {
                auto pTexture = assetManager.GetTexture( "scene_icon" );
                if ( pTexture )
                    return pTexture->GetID();

                break;
            }
        }

        return 0;
    }

    bool AssetDisplay::DoRenameAsset(const std::string &sOldName, const std::string &sNewName) const {
        if ( sNewName.empty() )
            return false;

        if ( m_SelectedType == otterus_utils::AssetType::SCENE )
        {
            // TODO: Change the scene name
        }
        else
        {
            auto& assetManager = MAIN_REGISTRY().GetAssetManager();
            return assetManager.ChangeAssetName( sOldName, sNewName, m_SelectedType );
        }

        OTTERUS_ASSERT( false && "How did it get here??" );
        return false;
    }

    bool AssetDisplay::DoDeleteAsset(const std::string &sAssetName) {
        if ( sAssetName.empty() )
            return false;

        if ( m_SelectedType == otterus_utils::AssetType::SCENE )
        {
            // TODO: Delete scene
            return false;
        }
        else
        {
            auto& assetManager = MAIN_REGISTRY().GetAssetManager();
            return assetManager.DeleteAsset( sAssetName, m_SelectedType );
        }

        return false;
    }

    void AssetDisplay::CheckRename(const std::string &sCheckName) const {
        if ( sCheckName.empty() )
        {
            ImGui::TextColored( ImVec4{ 1.f, 0.f, 0.f, 1.f }, "Rename text cannot be blank!" );
            return;
        }

        bool bHasAsset{ false };

        if ( m_SelectedType == otterus_utils::AssetType::SCENE )
        {
            // TODO: Check is scene name already exists
        }
        else
        {
            auto& assetManager = MAIN_REGISTRY().GetAssetManager();
            if ( assetManager.CheckHasAsset( sCheckName, m_SelectedType ) )
                bHasAsset = true;
        }

        if ( bHasAsset )
            ImGui::TextColored( ImVec4{ 1.f, 0.f, 0.f, 1.f },
                                fmt::format( "Asset name [{}] already exists!", sCheckName ).c_str() );
    }

    AssetDisplay::AssetDisplay()
    : m_ItemHovered { false }
    , m_AssetTypeChanged{ true }
    , m_Rename { false }
    , m_WindowSelected { false }
    , m_WindowHovered { false }
    , m_SelectedType {otterus_utils::AssetType::TEXTURE}
    , m_SelectedAssetName { "" }
    , m_SelectedAssetType { "TEXTURES" }
    , m_DragSource { "" }
    , m_RenameBuf { "" }
    , m_AssetSize { DEFAULT_ASSET_SIZE }
    , m_SelectedID{ -1 } {
    	SetAssetType();
    }

    void AssetDisplay::Update() {
        if ( !m_WindowHovered )
            return;

        auto& inputManager = otterus_core::InputManager::GetInstance();
        auto& keyboard = inputManager.GetKeyboard();
        auto& mouse = inputManager.GetMouse();

        const auto& io = ImGui::GetIO();
        bool bCtrl = io.KeyCtrl || keyboard.IsKeyPressed( OT_KEY_LCTRL ) || keyboard.IsKeyPressed( OT_KEY_RCTRL );

        if ( !bCtrl )
            return;

        float wheel = static_cast<float>( mouse.GetMouseWheelY() );
        if ( wheel == 0.f )
            wheel = io.MouseWheel;

        if ( wheel != 0.f )
        {
            m_AssetSize += wheel * ASSET_SIZE_STEP;
            m_AssetSize = std::clamp( m_AssetSize, MIN_ASSET_SIZE, MAX_ASSET_SIZE );
        }
    }

    void AssetDisplay::Draw() {
        if ( !ImGui::Begin( "Assets" ) )
        {
            m_WindowHovered = false;
            m_WindowSelected = false;
            ImGui::End();
            return;
        }

        m_WindowHovered = ImGui::IsWindowHovered( ImGuiHoveredFlags_RootAndChildWindows );
        m_WindowSelected = ImGui::IsWindowFocused( ImGuiFocusedFlags_RootAndChildWindows );

        auto& mainRegistry = MAIN_REGISTRY();
        auto& assetManager = mainRegistry.GetAssetManager();

        ImGui::Text( "Asset Type" );
        ImGui::SameLine( 0.f, 10.f );
        if ( ImGui::BeginCombo( "##AssetType", m_SelectedAssetType.c_str() ) )
        {

            for ( const auto& AssetType : m_SelectableTypes )
            {
                bool isSelected = m_SelectedAssetType == AssetType;
                if ( ImGui::Selectable( AssetType.c_str(), isSelected ) )
                {
                    m_AssetTypeChanged = true;
                    m_SelectedAssetType = AssetType;
                    m_SelectedID = -1;
                    SetAssetType();
                }

                if ( isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if ( ImGui::BeginChild( "##AssetTable",
                                ImVec2{ 0.f, 0.f },
                                NULL,
                                ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_ChildWindow ) )
        {
            m_WindowHovered = ImGui::IsWindowHovered();
            m_WindowSelected = ImGui::IsWindowFocused();

            DrawSelectedAssets();

            ImGui::EndChild();
        }

        ImGui::End();
    }
}
