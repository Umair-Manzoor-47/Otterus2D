#pragma once
#include <memory>

#include "AbstractTool.h"
#include "Core/ECS/Entity.h"

namespace otterus_rendering{ class SpriteBatchRenderer; }

namespace otterus_editor
{
    class TileTool : public  AbstractTool
    {
    private:
        glm::vec2 m_MouseRect;
        glm::vec2 m_GridCoords;

        bool m_GridSnap;

    protected:
        std::shared_ptr<otterus_rendering::SpriteBatchRenderer> m_BatchRenderer;
        std::shared_ptr<struct Tile> m_MouseTile;

    protected:
        uint32_t CheckForTile(const glm::vec2& position);

        otterus_core::ECS::Entity CreateEntity();
        otterus_core::ECS::Entity CreateEntity(uint32_t id);

        void DrawMouseSprite();
        virtual void ExamineMousePosition() override;
    public:
        TileTool();
        virtual ~TileTool() = default;

        virtual void Create() = 0;
        virtual void Draw() = 0;
	    virtual void Update( Canvas& canvas ) override;

        void ClearMouseTextureData();
        void LoadSpriteTextureData(const std::string& textureName);
        const std::string& GetSpriteTexture() const;

        void SetBoxCollider(int width, int height, const glm::vec2& offset);
        void SetSpriteLayer(int layer);
        void SetScale(const glm::vec2& scale);
        void SetAnimation(int numFrames, int frameRate, bool isVertical, bool looped, int frameOffset);
        void SetSpriteUVs(int startX, int startY);

        // TODO: Physics Props

        void SetSpriteRect(glm::vec2& spriteRect);

        void SetCollider( bool collider );
        void SetCircle( bool circle );
        void SetAnimation( bool animated );
        void SetPhysics( bool physics );

        const bool SpriteValid() const;

        inline void EnableGridSnap () {m_GridSnap = true; }
        inline void DisableGridSnap () {m_GridSnap = false;}
        const bool CanDrawOrCreate() const;

        inline Tile& GetTileData() { return *m_MouseTile; }
    };
}
