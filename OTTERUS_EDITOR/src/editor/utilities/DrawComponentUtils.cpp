#include "DrawComponentUtils.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/Resources/AssetManager.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include "OtterusUtilities/OtterusUtilities.h"
#include "Logger/Logger.h"
#include "editor/utilities/EditorUtilities.h"
#include "editor/utilities/ImGuiUtils.h"

#include <algorithm>
#include <cstring>

using namespace otterus_core::ECS;

namespace otterus_editor
{
namespace
{
	void DrawTextProperty( const char* propLabel, const char* id, std::string& value )
	{
		char buf[256]{};
		std::strncpy( buf, value.c_str(), sizeof( buf ) - 1 );
		ui::PropertyLabel( propLabel );
		const bool submitted = ImGui::InputText( id, buf, sizeof( buf ), ImGuiInputTextFlags_EnterReturnsTrue );
		if ( submitted || ImGui::IsItemDeactivatedAfterEdit() )
		{
			value = buf;
		}
	}
}

void DrawComponentUtils::DrawImGuiComponent( TransformComponent& transform )
{
	ImGui::PushID( entt::type_hash<TransformComponent>::value() );
	if ( ui::ComponentSectionHeader( "Transform" ) )
	{
		if ( ui::BeginPropertyTable( "##TransformProps" ) )
		{
			ui::PropertyLabel( "Position" );
			ui::DragFloat2Axis( "##Position", &transform.position.x, &transform.position.y, 1.0f, 0.0f, 0.0f, "%.1f" );

			ui::PropertyLabel( "Rotation" );
			ImGui::DragFloat( "##Rotation", &transform.rotation, 0.5f, 0.0f, 0.0f, "%.1f" );

			ui::PropertyLabel( "Scale" );
			ui::DragFloat2Axis( "##Scale", &transform.scale.x, &transform.scale.y, 0.1f, 0.1f, 150.0f, "%.2f" );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( SpriteComponent& sprite )
{
	bool changed{ false };

	ImGui::PushID( entt::type_hash<SpriteComponent>::value() );
	if ( ui::ComponentSectionHeader( "Sprite Renderer" ) )
	{
		if ( ui::BeginPropertyTable( "##SpriteProps" ) )
		{
			ui::PropertyLabel( "Sprite" );
			auto& assetManager = MAIN_REGISTRY().GetAssetManager();
			if ( ImGui::BeginCombo( "##texture", sprite.texture_name.c_str() ) )
			{
				for ( const auto& textureName : assetManager.GetAssetKeyNames( otterus_utils::AssetType::TEXTURE ) )
				{
					if ( ImGui::Selectable( textureName.c_str(), textureName == sprite.texture_name ) )
					{
						sprite.texture_name = textureName;
						changed = true;
					}
				}
				ImGui::EndCombo();
			}

			if ( ImGui::BeginDragDropTarget() )
			{
				if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( DROP_TEXTURE_SRC ) )
				{
					const std::string textureStr{ static_cast<const char*>( payload->Data ) };
					OTTERUS_ASSERT( !textureStr.empty() && "Texture Name is Empty!" );
					if ( !textureStr.empty() )
					{
						sprite.texture_name = textureStr;
						changed = true;
					}
				}
				ImGui::EndDragDropTarget();
			}

			ui::PropertyLabel( "Color" );
			ui::ColorEdit4U8( "##color", sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a );

			ui::PropertyLabel( "Width" );
			changed |= ImGui::DragFloat( "##width", &sprite.width, 1.0f, 8.f, 1366.f, "%.1f", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Height" );
			changed |= ImGui::DragFloat( "##height", &sprite.height, 1.0f, 8.f, 768.f, "%.1f", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Sorting Layer" );
			ImGui::DragInt( "##layer", &sprite.layer, 0.25f, 0, 10, "%d", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Sheet Pos" );
			changed |= ui::DragInt2Axis( "##SheetPos", &sprite.start_x, &sprite.start_y, 0.25f, 0, 32 );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();

	if ( changed )
	{
		if ( auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name ) )
		{
			otterus_core::GenerateUVs( sprite, texture->GetWidth(), texture->GetHeight() );
		}
	}
}

void DrawComponentUtils::DrawImGuiComponent( AnimationComponent& animation )
{
	ImGui::PushID( entt::type_hash<AnimationComponent>::value() );
	if ( ui::ComponentSectionHeader( "Animation" ) )
	{
		if ( ui::BeginPropertyTable( "##AnimationProps" ) )
		{
			ui::PropertyLabel( "Frames" );
			ImGui::DragInt( "##num_frames", &animation.numFrames, 0.25f, 1, 15, "%d", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Frame Rate" );
			ImGui::DragInt( "##frame_rate", &animation.frameRate, 0.25f, 1, 25, "%d", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Frame Offset" );
			ImGui::DragInt( "##frame_offset", &animation.frameOffset, 0.25f, 0, 15, "%d", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Vertical" );
			ui::ToggleSwitch( "##vertical", &animation.vertical );

			ui::PropertyLabel( "Looped" );
			ui::ToggleSwitch( "##looped", &animation.looped );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( BoxColliderComponent& boxCollider )
{
	ImGui::PushID( entt::type_hash<BoxColliderComponent>::value() );
	if ( ui::ComponentSectionHeader( "Box Collider 2D" ) )
	{
		if ( ui::BeginPropertyTable( "##BoxColliderProps" ) )
		{
			ui::PropertyLabel( "Width" );
			ImGui::DragInt( "##width", &boxCollider.width, 1.0f, 4, 1366, "%d", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Height" );
			ImGui::DragInt( "##height", &boxCollider.height, 1.0f, 4, 768, "%d", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Offset" );
			ui::DragFloat2Axis( "##Offset", &boxCollider.offset.x, &boxCollider.offset.y, 0.5f, 0.f, 128.f, "%.1f" );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( CircleColliderComponent& circleCollider )
{
	ImGui::PushID( entt::type_hash<CircleColliderComponent>::value() );
	if ( ui::ComponentSectionHeader( "Circle Collider 2D" ) )
	{
		if ( ui::BeginPropertyTable( "##CircleColliderProps" ) )
		{
			ui::PropertyLabel( "Radius" );
			ImGui::DragFloat( "##radius", &circleCollider.radius, 0.5f, 4.f, 1366.f, "%.1f", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Offset" );
			ui::DragFloat2Axis( "##Offset", &circleCollider.offset.x, &circleCollider.offset.y, 0.5f, 0.f, 128.f, "%.1f" );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( PhysicsComponent& physics )
{
	ImGui::PushID( entt::type_hash<PhysicsComponent>::value() );
	if ( ui::ComponentSectionHeader( "Physics 2D" ) )
	{
		ImGui::TextDisabled( "No configurable properties." );
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( RigidBodyComponent& rigidbody )
{
	ImGui::PushID( entt::type_hash<RigidBodyComponent>::value() );
	if ( ui::ComponentSectionHeader( "Rigidbody 2D" ) )
	{
		ImGui::TextDisabled( "No configurable properties." );
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( TextComponent& textComponent )
{
	ImGui::PushID( entt::type_hash<TextComponent>::value() );
	if ( ui::ComponentSectionHeader( "Text" ) )
	{
		if ( ui::BeginPropertyTable( "##TextProps" ) )
		{
			DrawTextProperty( "Text", "##_textStr", textComponent.textStr );

			ui::PropertyLabel( "Font" );
			if ( ImGui::BeginCombo( "##fontName", textComponent.fontName.c_str() ) )
			{
				auto& assetManager = MAIN_REGISTRY().GetAssetManager();
				for ( const auto& sFont : assetManager.GetAssetKeyNames( otterus_utils::AssetType::FONT ) )
				{
					if ( ImGui::Selectable( sFont.c_str(), sFont == textComponent.fontName ) )
					{
						textComponent.fontName = sFont;
					}
				}
				ImGui::EndCombo();
			}

			ui::PropertyLabel( "Padding" );
			ImGui::DragInt( "##padding", &textComponent.padding, 0.25f );

			ui::PropertyLabel( "Wrap" );
			ImGui::DragFloat( "##textWrap", &textComponent.wrap, 0.5f, 0.f, 0.f, "%.1f" );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( Identification& identification )
{
	ImGui::PushID( entt::type_hash<Identification>::value() );
	if ( ui::ComponentSectionHeader( "Identification" ) )
	{
		if ( ui::BeginPropertyTable( "##IdentificationProps" ) )
		{
			DrawTextProperty( "Name", "##_name", identification.name );
			DrawTextProperty( "Tag / Group", "##_group", identification.group );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();
}

void DrawComponentUtils::DrawImGuiComponent( TileComponent& tileComponent )
{}

void DrawComponentUtils::DrawImGuiComponent( LightComponent& light )
{
	ImGui::PushID( entt::type_hash<LightComponent>::value() );
	if ( ui::ComponentSectionHeader( "Light 2D" ) )
	{
		if ( ui::BeginPropertyTable( "##LightProps" ) )
		{
			ui::PropertyLabel( "Color" );
			ui::ColorEdit4U8( "##light_color", light.color.r, light.color.g, light.color.b, light.color.a );

			ui::PropertyLabel( "Offset" );
			ui::DragFloat2Axis( "##Offset", &light.offset.x, &light.offset.y, 0.5f, 0.f, 0.f, "%.1f" );

			ui::PropertyLabel( "Intensity" );
			ImGui::DragFloat( "##light_intensity", &light.intensity, 0.05f, 0.0f, 100.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Radius" );
			ImGui::DragFloat( "##light_radius", &light.radius, 1.0f, 1.0f, 5000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Falloff" );
			ImGui::DragFloat( "##light_falloff", &light.falloff, 0.05f, 0.1f, 100.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp );

			ui::PropertyLabel( "Spotlight" );
			ui::ToggleSwitch( "##light_spotlight", &light.isSpotlight );

			if ( light.isSpotlight )
			{
				ui::PropertyLabel( "Inner Angle" );
				if ( ImGui::DragFloat( "##light_inner_angle", &light.innerAngle, 0.5f, 0.0f, 180.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp ) )
				{
					light.outerAngle = std::max( light.outerAngle, light.innerAngle );
				}

				ui::PropertyLabel( "Outer Angle" );
				ImGui::DragFloat( "##light_outer_angle", &light.outerAngle, 0.5f, light.innerAngle, 180.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp );
			}

			ui::PropertyLabel( "Cast Shadows" );
			ui::ToggleSwitch( "##light_cast_shadows", &light.castShadows );

			ui::EndPropertyTable();
		}
	}
	ImGui::PopID();
}

}
