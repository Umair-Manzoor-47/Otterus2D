#pragma once
#include "Core/Loaders/TilemapLoader.h"

namespace otterus_core::Loaders
{
    bool TilemapLoader::SaveTilemapJSON(ECS::Registry& registry, const std::string& tilemapFile)
    {
        return false;
    }

    bool TilemapLoader::LoadTilemapJSON(ECS::Registry& registry, const std::string& tilemapFile)
    {
        return false;
    }

    bool TilemapLoader::SaveTilemap(ECS::Registry& registry, const std::string& tilemapFile, bool useJSON)
    {
        if (useJSON)
            return SaveTilemapJSON(registry, tilemapFile);
        return false;
    }

    bool TilemapLoader::LoadTilemap(ECS::Registry& registry, const std::string& tilemapFile, bool useJSON)
    {
        if (useJSON)
            return LoadTilemapJSON(registry, tilemapFile);
        return false;
    }
}
