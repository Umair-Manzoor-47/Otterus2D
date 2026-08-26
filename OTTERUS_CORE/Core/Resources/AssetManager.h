#pragma once

#include <map>
#include <memory>
#include <string>


#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>
#include <Rendering/Essentials/Font.h>
#include <Sounds/Essentials/Music.h>
#include <Sounds/Essentials/SoundFX.h>
#include <sol/sol.hpp>
#include "../ECS/Registry.h"

namespace otterus_resources {
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<otterus_rendering::Texture>> m_mapTextures;
		std::map<std::string, std::shared_ptr<otterus_rendering::Shader>> m_mapShaders;
		std::map<std::string, std::shared_ptr<otterus_rendering::Font>> m_mapFonts;
		std::map<std::string, std::shared_ptr<otterus_sounds::Music>> m_mapMusic;
		std::map<std::string, std::shared_ptr<otterus_sounds::SoundFX>> m_mapSoundFX;

	public:
		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string & textureName, const std::string & texturePath, bool pixelArt);
		const otterus_rendering::Texture& GetTexture(const std::string& textureName);

		bool AddShader(const std::string & shaderName, const std::string & vertexPath, const std::string & fragmentPath);
		otterus_rendering::Shader& GetShader(const std::string& shaderName);

		bool AddFont(const std::string& fontName, const std::string& fontPath, float fontSize = 32.f);
		bool AddFontFromMemory(const std::string& fontName, unsigned char* fontData, float fontSize = 32.f);
		std::shared_ptr<otterus_rendering::Font> GetFont(const std::string& fontName);

		bool AddMusic(const std::string& musicName, const std::string& filePath);
		std::shared_ptr<otterus_sounds::Music> GetMusic(const std::string& musicName);

		bool AddSoundFX(const std::string& soundFXName, const std::string& filePath);
		std::shared_ptr<otterus_sounds::SoundFX> GetSoundFX(const std::string& soundFXName);

		static void CreateLuaAssetManager(sol::state& lua, otterus_core::ECS::Registry& registry);
	};


}