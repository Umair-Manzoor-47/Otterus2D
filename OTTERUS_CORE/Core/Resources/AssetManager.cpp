#include "AssetManager.h"
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Logger/Logger.h>


namespace otterus_resources {

    bool otterus_resources::AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt)
    {
        if (m_mapTextures.find(textureName) != m_mapTextures.end()) {

            OTTERUS_LOG("Failed to add texture [{0}] -- Texture already exists.", textureName);
            return false;
        }

        auto texture = std::move(otterus_rendering::TextureLoader::Create(texturePath,
            pixelArt ? otterus_rendering::Texture::TextureType::PIXEL : otterus_rendering::Texture::TextureType::BLENDED
            ));

        if (!texture) {

            OTTERUS_LOG("Failed to load texture [{0}] -- At path {1}.", textureName, texturePath);
            return false;
        }

        m_mapTextures.emplace(textureName, std::move(texture));

        return true;
    }

    const otterus_rendering::Texture& AssetManager::GetTexture(const std::string& textureName)
    {
        auto texItr = m_mapTextures.find(textureName);
        if (texItr == m_mapTextures.end()) {
        
            OTTERUS_ERROR("Failed to get texture [{0}] -- Does not exist.", textureName);
            return otterus_rendering::Texture();
        }

        return *texItr->second;
    }

    bool otterus_resources::AssetManager::AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
    {
        if (m_mapShaders.find(shaderName) != m_mapShaders.end()) {

            OTTERUS_LOG("Failed to add shader [{0}] -- Shader already exists.", shaderName);
            return false;
        }

        auto shader = std::move(otterus_rendering::ShaderLoader::Create(vertexPath, fragmentPath));

        if (!shader) {

            OTTERUS_LOG("Failed to load shader [{0}] -- At vertex path {1} and fragment path {2}.", 
                shaderName, vertexPath, fragmentPath);
            return false;
        }

        m_mapShaders.emplace(shaderName, std::move(shader));

        return true;
    }

    otterus_rendering::Shader& AssetManager::GetShader(const std::string& shaderName)
    {
        auto shaderItr = m_mapShaders.find(shaderName);
        if (shaderItr == m_mapShaders.end()) {

            OTTERUS_ERROR("Failed to get shader [{0}] -- Does not exist.", shaderName);
            otterus_rendering::Shader shader{};
            return shader;
        }

        return *shaderItr->second;
    }
    bool AssetManager::AddMusic(const std::string& musicName, const std::string& filePath)
    {
        if (m_mapMusic.find(musicName) != m_mapMusic.end()) {

            OTTERUS_LOG("Failed to add music [{0}] -- Music already exists.", musicName);
            return false;
        }

        Mix_Music* music = Mix_LoadMUS(filePath.c_str());
        if (!music) {
            std::string error{ Mix_GetError() };
            OTTERUS_LOG("Failed to add music [{0}] -- Mix_error {}.", musicName, error);
            return false;
        }

        otterus_sounds::SoundParams params{
            .name = musicName,
            .filename = filePath,
            .duration = Mix_MusicDuration(music),
        };

        auto musicPtr = std::make_shared<otterus_sounds::Music>(params, MusicPtr{ music });

        if (!musicPtr) {
            OTTERUS_LOG("Failed to create music pointer for [{0}]", musicName);
            return false;
        }

        m_mapMusic.emplace(musicName, std::move(musicPtr));

        return true;
    }
    std::shared_ptr<otterus_sounds::Music> AssetManager::GetMusic(const std::string& musicName)
    {
        auto musicItr = m_mapMusic.find(musicName);
        if (musicItr == m_mapMusic.end()) {

            OTTERUS_ERROR("Failed to get music [{0}] -- Does not exist.", musicName);
            return nullptr;
        }

        return musicItr->second;
    }

    bool AssetManager::AddSoundFX(const std::string& soundFXName, const std::string& filePath)
    {
        if (m_mapSoundFX.find(soundFXName) != m_mapSoundFX.end()) {

            OTTERUS_LOG("Failed to add SoundFX [{0}] -- SoundFX already exists.", soundFXName);
            return false;
        }

        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if (!chunk) {
            std::string error{ Mix_GetError() };
            OTTERUS_LOG("Failed to add soundFX [{0}] -- Mix_error {}.", soundFXName, error);
            return false;
        }

        otterus_sounds::SoundParams params{
            .name = soundFXName,
            .filename = filePath,
            .duration = chunk->alen/179.4,
        };

        auto soundFXPtr = std::make_shared<otterus_sounds::SoundFX>(params, SoundFxPtr{ chunk });

        if (!soundFXPtr) {
            OTTERUS_LOG("Failed to create soundFX pointer for [{0}]", soundFXName);
            return false;
        }

        m_mapSoundFX.emplace(soundFXName, std::move(soundFXPtr));

        return true;
    }

    std::shared_ptr<otterus_sounds::SoundFX> AssetManager::GetSoundFX(const std::string& soundFXName)
    {
        auto soundFXItr = m_mapSoundFX.find(soundFXName);
        if (soundFXItr == m_mapSoundFX.end()) {

            OTTERUS_ERROR("Failed to get soundFX [{0}] -- Does not exist.", soundFXName);
            return nullptr;
        }

        return soundFXItr->second;
    }

    void AssetManager::CreateLuaAssetManager(sol::state& lua, otterus_core::ECS::Registry& registry)
    {
        auto& assetManager = registry.GetContext <std::shared_ptr<AssetManager>>();
        if (!assetManager) {
            OTTERUS_ERROR("AssetManager does not exists in Registry.");
            return;
        }

        lua.new_usertype<AssetManager>(
            "AssetManager",
            sol::no_constructor,
            "add_texture", [&](const std::string& assetName, const std::string& filepath, bool pixel_art) {
                return assetManager->AddTexture(assetName, filepath, pixel_art);
            },
            "add_music", [&](const std::string& musicName, const std::string& filepath){
                return assetManager->AddMusic(musicName, filepath);
            },
            "add_sound", [&](const std::string& soundFXName, const std::string& filepath) {
                return assetManager->AddSoundFX(soundFXName, filepath);
            }
        
        );
    }
}