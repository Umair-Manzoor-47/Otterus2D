#include "DrawComponentUtils.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/Resources/AssetManager.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include "OtterusUtilities/OtterusUtilities.h"
#include "Logger/Logger.h"

#include "editor/utilities/ImGuiUtils.h"

using namespace otterus_core::ECS;

namespace otterus_editor
{
void DrawComponentUtils::DrawImGuiComponent( TransformComponent& transform )
{
	ImGui::SeparatorText( "Transform" );
	ImGui::PushID(entt::type_hash<TransformComponent>::value());
	if (ImGui::TreeNodeEx("##TranformTree", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth( 120.f );
		ImGui::InlineLabel( "position" );
		ImGui::ColoredLabel( "x", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		ImGui::InputFloat( "##position_x", &transform.position.x, 1.f, 10.f, "%.1f" );
		ImGui::SameLine();
		ImGui::ColoredLabel( "y", LABEL_SINGLE_SIZE, LABEL_GREEN );
		ImGui::SameLine();
		ImGui::InputFloat( "##position_y", &transform.position.y, 1.f, 10.f, "%.1f" );

		ImGui::InlineLabel( "scale" );
		ImGui::ColoredLabel( "x", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		if (ImGui::InputFloat("##scale_x", &transform.scale.x, 1.f, 1.f, "%.1f"))
		{
			transform.scale.x = std::clamp( transform.scale.x, 0.1f, 150.f );
		}
		ImGui::SameLine();
		ImGui::ColoredLabel( "y", LABEL_SINGLE_SIZE, LABEL_GREEN );
		ImGui::SameLine();
		if (ImGui::InputFloat("##scale_y", &transform.scale.y, 1.f, 1.f, "%.1f"))
		{
			transform.scale.y = std::clamp( transform.scale.y, 0.1f, 150.f );
		}

		ImGui::InlineLabel( "rotation" );
		ImGui::InputFloat( "##rotation", &transform.rotation, 1.f, 1.f, "%.1f" );
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}

	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( SpriteComponent& sprite )
{
	bool changed{ false };

	ImGui::SeparatorText( "Sprite" );
	ImGui::PushID( entt::type_hash<SpriteComponent>::value() );
	if ( ImGui::TreeNodeEx( "##SpriteTree", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		ImGui::AddSpaces( 2 );
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "AssetDisplayTexture" );

			if (payload)
			{
				const char* texture = (const char*)payload->Data;
				std::string textureStr{ texture };
				OTTERUS_ASSERT( !textureStr.empty() && "Texture Name is Empty!" );
				if (!textureStr.empty())
				{
					sprite.texture_name = textureStr;
				}
			}

			ImGui::EndDragDropTarget();
		}

		// Color picker
		ImVec4 col = { sprite.color.r / 255.f, sprite.color.g / 255.f, sprite.color.b / 255.f, sprite.color.a / 255.f };
		ImGui::InlineLabel( "color" );
		if (ImGui::ColorEdit4("##color", &col.x, IMGUI_COLOR_PICKER_FLAGS))
		{
			sprite.color.r = static_cast<GLubyte>( col.x * 255.f );
			sprite.color.g = static_cast<GLubyte>( col.y * 255.f );
			sprite.color.b = static_cast<GLubyte>( col.z * 255.f );
			sprite.color.a = static_cast<GLubyte>( col.w * 255.f );
		}

		auto& assetManager = MAIN_REGISTRY().GetAssetManager();

		std::string selectedTexture{ sprite.texture_name };
		ImGui::InlineLabel( "texture" );
		if (ImGui::BeginCombo("##texture", selectedTexture.c_str()))
		{
			for (const auto& textureName : assetManager.GetAssetKeyNames(otterus_utils::AssetType::TEXTURE))
			{
				if (ImGui::Selectable(textureName.c_str(), textureName == selectedTexture))
				{
					selectedTexture = textureName;
					sprite.texture_name = selectedTexture;
					changed = true;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::AddSpaces( 2 );

		ImGui::PushItemWidth( 120.f );
		ImGui::InlineLabel( "width" );
		if (ImGui::InputFloat("##width", &sprite.width, 8.f, 8.f))
		{
			sprite.width = std::clamp( sprite.width, 8.f, 1366.f );
			changed = true;
		}

		ImGui::InlineLabel( "height" );
		if ( ImGui::InputFloat( "##height", &sprite.height, 8.f, 8.f ) )
		{
			sprite.height = std::clamp( sprite.height, 8.f, 768.f );
			changed = true;
		}

		ImGui::InlineLabel( "layer" );
		if ( ImGui::InputInt( "##layer", &sprite.layer, 1, 1 ) )
		{
			sprite.layer = std::clamp( sprite.layer, 0, 10);
		}

		ImGui::InlineLabel( "Sprite Sheet Position" );
		ImGui::ColoredLabel( "x", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		if (ImGui::InputInt("##start_x", &sprite.start_x, 1, 1))
		{
			sprite.start_x = std::clamp( sprite.start_x, 0, 32 );
			changed = true;
		}
		ImGui::SameLine();
		ImGui::ColoredLabel( "y", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		if ( ImGui::InputInt( "##start_y", &sprite.start_y, 1, 1 ) )
		{
			sprite.start_y = std::clamp( sprite.start_y, 0, 32 );
			changed = true;
		}

		ImGui::TreePop();
		ImGui::PopItemWidth();
	}

	ImGui::PopID();

	if (changed)
	{
		auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
		if ( !texture )
			return;

		otterus_core::GenerateUVs(sprite, texture->GetWidth(), texture->GetHeight());
	}
}

void DrawComponentUtils::DrawImGuiComponent( AnimationComponent& animation )
{
	ImGui::SeparatorText( "Animation Component" );
	ImGui::PushID( entt::type_hash<AnimationComponent>::value() );
	if ( ImGui::TreeNodeEx( "", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		ImGui::PushItemWidth( 120.f );
		ImGui::InlineLabel( "num frames" );
		if ( ImGui::InputInt( "##1", &animation.numFrames, 1, 1 ) )
			animation.numFrames = std::clamp( animation.numFrames, 1, 15 );

		ImGui::InlineLabel( "frame rate" );
		if ( ImGui::InputInt( "##frame_rate", &animation.frameRate, 1, 1 ) )
			animation.frameRate = std::clamp( animation.frameRate, 1, 25 );

		ImGui::InlineLabel( "frame offset" );
		if ( ImGui::InputInt( "##frame_offset", &animation.frameOffset, 1, 1 ) )
			animation.frameOffset = std::clamp( animation.frameOffset, 0, 15 );

		ImGui::InlineLabel( "vertical" );
		ImGui::Checkbox( "##vertical", &animation.vertical );
		ImGui::InlineLabel( "looped" );
		ImGui::Checkbox( "##looped", &animation.looped );
		ImGui::TreePop();
		ImGui::PopItemWidth();
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( BoxColliderComponent& boxCollider )
{
	ImGui::SeparatorText( "Box Collider Component" );
	ImGui::PushID( entt::type_hash<BoxColliderComponent>::value() );
	if ( ImGui::TreeNodeEx( "", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		ImGui::PushItemWidth( 120.f );
		ImGui::InlineLabel( "width" );
		if ( ImGui::InputInt( "##width", &boxCollider.width, 4, 4 ) )
			boxCollider.width = std::clamp( boxCollider.width, 4, 1366 );

		ImGui::InlineLabel( "height" );
		if ( ImGui::InputInt( "##height", &boxCollider.height, 4, 4 ) )
			boxCollider.height = std::clamp( boxCollider.height, 4, 768 );

		ImGui::InlineLabel( "offset" );
		ImGui::ColoredLabel( "x", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		if ( ImGui::InputFloat( "##offset_x", &boxCollider.offset.x, 4.f, 4.f ) )
			boxCollider.offset.x = std::clamp( boxCollider.offset.x, 0.f, 128.f );
		ImGui::SameLine();
		ImGui::ColoredLabel( "y", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		if ( ImGui::InputFloat( "##offset_y", &boxCollider.offset.y, 4.f, 4.f ) )
			boxCollider.offset.y = std::clamp( boxCollider.offset.y, 0.f, 128.f );
		ImGui::TreePop();
		ImGui::PopItemWidth();
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( CircleColliderComponent& circleCollider )
{
	ImGui::SeparatorText( "Circle Collider Component" );
	ImGui::PushID( entt::type_hash<CircleColliderComponent>::value() );
	if ( ImGui::TreeNodeEx( "", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		ImGui::PushItemWidth( 120.f );
		ImGui::InlineLabel( "radius" );
		if ( ImGui::InputFloat( "##radius", &circleCollider.radius, 4, 4 ) )
			circleCollider.radius = std::clamp( circleCollider.radius, 4.f, 1366.f );

		ImGui::InlineLabel( "offset" );
		ImGui::ColoredLabel( "x", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		if ( ImGui::InputFloat( "##offset_x", &circleCollider.offset.x, 4.f, 4.f ) )
			circleCollider.offset.x = std::clamp( circleCollider.offset.x, 0.f, 128.f );
		ImGui::SameLine();
		ImGui::ColoredLabel( "y", LABEL_SINGLE_SIZE, LABEL_RED );
		ImGui::SameLine();
		if ( ImGui::InputFloat( "##offset_y", &circleCollider.offset.y, 4.f, 4.f ) )
			circleCollider.offset.y = std::clamp( circleCollider.offset.y, 0.f, 128.f );
		ImGui::TreePop();
		ImGui::PopItemWidth();
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( PhysicsComponent& physics )
{
	ImGui::SeparatorText( "Physics Component" );
	ImGui::PushID( entt::type_hash<PhysicsComponent>::value() );
	if ( ImGui::TreeNodeEx( "", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		// TODO: Fill in when Component is ready
		ImGui::TreePop();
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( RigidBodyComponent& rigidbody )
{
	ImGui::SeparatorText( "Rigidbody Component" );
	ImGui::PushID( entt::type_hash<RigidBodyComponent>::value() );
	if ( ImGui::TreeNodeEx( "", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		// TODO: Fill in when Component is ready
		ImGui::TreePop();
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( TextComponent& textComponent )
{
	ImGui::SeparatorText( "Text Component" );
	ImGui::PushID( entt::type_hash<TextComponent>::value() );
	if ( ImGui::TreeNodeEx( "", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		std::string sTextBuffer{ textComponent.textStr };
		ImGui::InlineLabel( "text" );
		if ( ImGui::InputText(
				 "##_textStr", sTextBuffer.data(), sizeof( char ) * 255, ImGuiInputTextFlags_EnterReturnsTrue ) )
		{
			textComponent.textStr = std::string{ sTextBuffer.data() };
		}

		std::string fontName{ textComponent.fontName };
		ImGui::PushItemWidth( 164.f );
		ImGui::InlineLabel( "font" );
		if ( ImGui::BeginCombo( "##fontName", fontName.c_str() ) )
		{
			auto& assetManager = MAIN_REGISTRY().GetAssetManager();
			for ( const auto& sFont : assetManager.GetAssetKeyNames( otterus_utils::AssetType::FONT ) )
			{
				if ( ImGui::Selectable( sFont.c_str(), sFont == fontName ) )
				{
					fontName = sFont;
					textComponent.fontName = fontName;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::PushItemWidth( 120.f );
		ImGui::InlineLabel( "padding" );
		if ( ImGui::InputInt( "##padding", &textComponent.padding, 0, 0 ) )
		{
			// TODO
		}

		ImGui::InlineLabel( "wrap" );
		if ( ImGui::InputFloat( "##textWrap", &textComponent.wrap, 0.f, 0.f ) )
		{
			// TODO
		}

		ImGui::PopItemWidth();
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( Identification& identification )
{
	ImGui::SeparatorText( "Identificaton" );
	ImGui::PushID( entt::type_hash<Identification>::value() );
	if ( ImGui::TreeNodeEx( "", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		std::string nameBuffer{ identification.name };
		ImGui::InlineLabel( "name" );
		if ( ImGui::InputText(
				 "##_name", nameBuffer.data(), sizeof( char ) * 255, ImGuiInputTextFlags_EnterReturnsTrue ) )
		{
			identification.name = std::string{ nameBuffer.data() };
		}

		std::string groupBuffer{ identification.group };
		ImGui::InlineLabel( "group" );
		if ( ImGui::InputText(
				 "##_group", groupBuffer.data(), sizeof( char ) * 255, ImGuiInputTextFlags_EnterReturnsTrue ) )
		{
			identification.group = std::string{ groupBuffer.data() };
		}

		ImGui::TreePop();
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( LightComponent& light )
{
	ImGui::SeparatorText( "Light" );
	ImGui::PushID( entt::type_hash<LightComponent>::value() );
	if ( ImGui::TreeNodeEx( "##LightTree", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		ImGui::PushItemWidth( 160.f );

		ImGui::InlineLabel( "color" );
		ImGui::ColorEdit4( "##light_color", &light.color.x );

		ImGui::InlineLabel( "intensity" );
		if ( ImGui::InputFloat( "##light_intensity", &light.intensity, 0.1f, 0.5f, "%.2f" ) )
			light.intensity = std::max( light.intensity, 0.0f );

		ImGui::InlineLabel( "radius" );
		if ( ImGui::InputFloat( "##light_radius", &light.radius, 5.f, 25.f, "%.1f" ) )
			light.radius = std::max( light.radius, 1.0f );

		ImGui::InlineLabel( "falloff" );
		if ( ImGui::InputFloat( "##light_falloff", &light.falloff, 0.1f, 0.5f, "%.2f" ) )
			light.falloff = std::max( light.falloff, 0.1f );

		ImGui::InlineLabel( "spotlight" );
		ImGui::Checkbox( "##light_spotlight", &light.isSpotlight );

		if ( light.isSpotlight )
		{
			ImGui::InlineLabel( "inner angle" );
			if ( ImGui::InputFloat( "##light_inner_angle", &light.innerAngle, 1.f, 5.f, "%.1f" ) )
			{
				light.innerAngle = std::clamp( light.innerAngle, 0.0f, 180.0f );
				light.outerAngle = std::max( light.outerAngle, light.innerAngle );
			}

			ImGui::InlineLabel( "outer angle" );
			if ( ImGui::InputFloat( "##light_outer_angle", &light.outerAngle, 1.f, 5.f, "%.1f" ) )
			{
				light.outerAngle = std::clamp( light.outerAngle, light.innerAngle, 180.0f );
			}
		}

		ImGui::InlineLabel( "cast shadows" );
		ImGui::Checkbox( "##light_cast_shadows", &light.castShadows );

		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	ImGui::PopID();
}

}