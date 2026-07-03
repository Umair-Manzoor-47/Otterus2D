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
}