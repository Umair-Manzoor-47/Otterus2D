#include "SpriteComponent.h"
#include <entt.hpp>
#include "../../Resources/AssetManager.h"
#include <logger/Logger.h>


using namespace otterus_resources;

void otterus_core::ECS::SpriteComponent::CreateStaticLuaBind(sol::state& lua, otterus_core::ECS::Registry& registry)
{

	lua.new_usertype<SpriteComponent>(
		"Sprite",
		"type_id", &entt::type_hash<SpriteComponent>::value,
		sol::call_constructor,
		sol::factories(
			[](const std::string& textureName, float width, float height, int start_x, int start_y, int layer)
			{
				return SpriteComponent{
				.width = width,
				.height = height,
				.uvs = UVs{},
				.color = otterus_rendering::Color{255, 255, 255, 255},
				.start_x = start_x,
				.start_y = start_y,
				.texture_name = textureName,
				.layer = layer

				};
			}
		
		),
		"texture_name", &SpriteComponent::texture_name,
		"width", &SpriteComponent::width,
		"height", &SpriteComponent::height,
		"start_x", &SpriteComponent::start_x,
		"start_y", &SpriteComponent::start_y,
		"layer", &SpriteComponent::layer,
		"generate_uvs", [&](SpriteComponent& sprite) {
			auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();
			auto& texture = assetManager->GetTexture(sprite.texture_name);

			if (texture.GetID() == 0) {
				OTTERUS_ERROR("Failed to generate UVS texture [{0}] does not exists or invalid.", sprite.texture_name);
				return;
			}

			sprite.generate_uvs(texture.GetWidth(), texture.GetHeight());
		}
	);
}
