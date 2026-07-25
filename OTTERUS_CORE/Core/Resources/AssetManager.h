#pragma once

#include <map>
#include <memory>
#include <string>


#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>
#include <sol/sol.hpp>
#include "../ECS/Registry.h"

namespace otterus_resources {
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<otterus_rendering::Texture>> m_mapTextures;
		std::map<std::string, std::shared_ptr<otterus_rendering::Shader>> m_mapShaders;

	public:
		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string & textureName, const std::string & texturePath, bool pixelArt);
		const otterus_rendering::Texture& GetTexture(const std::string& textureName);

		bool AddShader(const std::string & shaderName, const std::string & vertexPath, const std::string & fragmentPath);
		otterus_rendering::Shader& GetShader(const std::string& shaderName);


		static void CreateLuaAssetManager(sol::state& lua, otterus_core::ECS::Registry& registry);
	};


}