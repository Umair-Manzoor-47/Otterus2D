#pragma once

#include "IDisplay.h"
#include <string>

namespace otterus_utils {
        enum class AssetType;
}

namespace otterus_editor
{
    class AssetDisplay : public IDisplay
    {
    private:
        const std::vector<std::string> m_SelectableTypes { "TEXTURES", "FONTS", "MUSIC", "SOUNDFX", "SCENES" };
        bool m_ItemHovered, m_AssetTypeChanged, m_Rename;
        bool m_WindowSelected, m_WindowHovered;
        otterus_utils::AssetType m_SelectedType;

        std::string m_SelectedAssetName, m_SelectedAssetType, m_DragSource, m_RenameBuf;
        float m_AssetSize;

        int m_SelectedID;

    private:
        void SetAssetType();
        void DrawSelectedAssets();
        unsigned int GetTextureID( const std::string& sAssetName ) const;
        bool DoRenameAsset( const std::string& sOldName, const std::string& sNewName ) const;
        bool DoDeleteAsset( const std::string& sAssetName );
        void CheckRename( const std::string& sCheckName ) const;

    public:
        AssetDisplay();
        ~AssetDisplay() = default;

        virtual void Draw() override;
        virtual void Update() override;
    };
}