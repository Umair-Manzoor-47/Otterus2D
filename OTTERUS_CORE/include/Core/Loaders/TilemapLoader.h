#pragma once
#include <string>

namespace otterus_core::ECS
{
    class Registry;
}

namespace otterus_core::Loaders
{
    class TilemapLoader
    {
    private:
        bool SaveTilemapJSON( ECS::Registry& registry, const std::string& tilemapFile );
        bool LoadTilemapJSON( ECS::Registry& registry, const std::string& tilemapFile );

        // TODO: Save and load functions for lua serializer

    public:
        TilemapLoader() = default;
        ~TilemapLoader() = default;

        bool SaveTilemap( ECS::Registry& registry, const std::string& tilemapFile, bool useJSON = false );
        bool LoadTilemap( ECS::Registry& registry, const std::string& tilemapFile, bool useJSON = false );
    };
}