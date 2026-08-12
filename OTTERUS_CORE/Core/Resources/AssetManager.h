#pragma once

#include <map>
#include <memory>
#include <string>


#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>
#include <Sounds/Essentials/Music.h>
#include <sol/sol.hpp>
#include "../ECS/Registry.h"

namespace otterus_resources {
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<otterus_rendering::Texture>> m_mapTextures;
		std::map<std::string, std::shared_ptr<otterus_rendering::Shader>> m_mapShaders;
		std::map<std::string, std::shared_ptr<otterus_sounds::Music>> m_mapMusic;

	public:
		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string & textureName, const std::string & texturePath, bool pixelArt);
		const otterus_rendering::Texture& GetTexture(const std::string& textureName);

		bool AddShader(const std::string & shaderName, const std::string & vertexPath, const std::string & fragmentPath);
		otterus_rendering::Shader& GetShader(const std::string& shaderName);

		bool AddMusic(const std::string& musicName, const std::string& filePath);
		std::shared_ptr<otterus_sounds::Music> GetMusic(const std::string& musicName);

		static void CreateLuaAssetManager(sol::state& lua, otterus_core::ECS::Registry& registry);
	};


}