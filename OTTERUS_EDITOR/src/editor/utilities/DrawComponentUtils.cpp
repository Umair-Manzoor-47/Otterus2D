#include "DrawComponentUtils.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/Resources/AssetManager.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include "OtterusUtilities/OtterusUtilities.h"
#include "Logger/Logger.h"
#include "editor/utilities/EditorUtilities.h"
#include "editor/utilities/ImGuiUtils.h"
#include "Physics/PhysicsUtilities.h"

#include <algorithm>
#include <cstring>

using namespace otterus_utils;
using namespace otterus_physics;
using namespace otterus_core::ECS;

namespace
{
	const char* GetPhysicsBodyDescription( RigidBodyType eType )
	{
		switch ( eType )
		{
		case RigidBodyType::STATIC: return "zero mass, zero velocity, may be manually moved";
		case RigidBodyType::KINEMATIC: return "zero mass, velocity set by user, moved by solver";
		case RigidBodyType::DYNAMIC: return "positive mass, velocity determined by forces, moved by solver";
		default: return "";
		}
	}
}

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
			ImGui::SetItemTooltip( "The radius of the circle component. This is clamped to [4 : 1366]" );

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
		PhysicsAttributes& physicsAttributes = physics.GetChangableAttributes();

		if ( ui::BeginPropertyTable( "##PhysicsProps" ) )
		{
			std::string sSelectedBodyType = GetRigidBodyTypeString( physicsAttributes.type );
			ui::PropertyLabel( "Body Type" );
			if ( ImGui::BeginCombo( "##body_type", sSelectedBodyType.c_str() ) )
			{
				for ( const auto& [ eBodyType, sBodyStr ] : GetRigidBodyStringMap() )
				{
					if ( ImGui::Selectable( sBodyStr.c_str(), sBodyStr == sSelectedBodyType ) )
					{
						physicsAttributes.type = eBodyType;
					}
					ImGui::SetItemTooltip( "%s", GetPhysicsBodyDescription( eBodyType ) );
				}
				ImGui::EndCombo();
			}
			ImGui::SetItemTooltip( "The body type: static, kinematic, or dynamic." );

			std::string sSelectedCategoryType =
				GetFilterCategoryString( static_cast<FilterCategory>( physicsAttributes.filterCategory ) );
			ui::PropertyLabel( "Category Type" );
			if ( ImGui::BeginCombo( "##category_type", sSelectedCategoryType.c_str() ) )
			{
				for ( const auto& [ eCategoryType, sCategoryStr ] : GetFilterCategoryToStringMap() )
				{
					if ( ImGui::Selectable( sCategoryStr.c_str(), sCategoryStr == sSelectedCategoryType ) )
					{
						physicsAttributes.filterCategory = static_cast<uint16_t>( eCategoryType );
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SetItemTooltip( "The collision category bits. Bodies will usually only use one category type." );

			static std::string sSelectedMaskBit{ "" };
			static FilterCategory eMaskCategory{ FilterCategory::NO_CATEGORY };
			ui::PropertyLabel( "Masks" );
			if ( ImGui::BeginCombo( "##mask_bits", sSelectedMaskBit.c_str() ) )
			{
				for ( const auto& [ eCategoryType, sCategoryStr ] : GetFilterCategoryToStringMap() )
				{
					if ( ImGui::Selectable( sCategoryStr.c_str(), sCategoryStr == sSelectedMaskBit ) )
					{
						sSelectedMaskBit = sCategoryStr;
						eMaskCategory = eCategoryType;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SetItemTooltip(
				"The collision mask bits. This states the categories that this shape would accept for collision." );

			if ( !sSelectedMaskBit.empty() && eMaskCategory != FilterCategory::NO_CATEGORY )
			{
				ui::PropertyLabel( "" );
				if ( ImGui::Button( "Apply Mask" ) )
				{
					uint16_t filterCat = static_cast<uint16_t>( eMaskCategory );
					if ( !IsBitSet( physicsAttributes.filterMask, filterCat ) )
					{
						physicsAttributes.filterMask += filterCat;
						eMaskCategory = FilterCategory::NO_CATEGORY;
						sSelectedMaskBit.clear();
					}
					else
					{
						OTTERUS_ERROR( "Masks already contain [{}]", sSelectedMaskBit );
					}
				}
			}

			if ( physicsAttributes.filterMask > 0 )
			{
				ui::PropertyLabel( "Applied Masks" );
				if ( ImGui::BeginListBox( "##applied_masks", ImVec2{ -FLT_MIN, 48.f } ) )
				{
					auto setMaskBits{ GetAllSetBits( physicsAttributes.filterMask ) };
					for ( auto mask : setMaskBits )
					{
						ImGui::Selectable(
							GetFilterCategoryString( static_cast<FilterCategory>( Bit( mask ) ) ).c_str() );
					}
					ImGui::EndListBox();
				}

				ui::PropertyLabel( "" );
				if ( ImGui::Button( "Clear Masks" ) )
				{
					physicsAttributes.filterMask = 0;
					eMaskCategory = FilterCategory::NO_CATEGORY;
					sSelectedMaskBit.clear();
				}
			}

			ui::PropertyLabel( "Density" );
			ImGui::InputFloat( "##density", &physicsAttributes.density, 1.f, 1.f, "%.1f" );
			ImGui::SetItemTooltip( "The density, usually in kg/m^2." );

			ui::PropertyLabel( "Friction" );
			if ( ImGui::InputFloat( "##friction", &physicsAttributes.friction, 0.1f, 0.1f, "%.1f" ) )
			{
				physicsAttributes.friction = std::clamp( physicsAttributes.friction, 0.f, 1.f );
			}
			ImGui::SetItemTooltip( "The Coulomb (dry) friction coefficient, usually in the range [0,1]." );

			ui::PropertyLabel( "Restitution" );
			if ( ImGui::InputFloat( "##restitution", &physicsAttributes.restitution, 0.1f, 0.1f, "%.1f" ) )
			{
				physicsAttributes.restitution = std::clamp( physicsAttributes.restitution, 0.f, 1.f );
			}
			ImGui::SetItemTooltip( "The restitution (bounce) usually in the range [0,1]." );

			ui::PropertyLabel( "Gravity Scale" );
			ImGui::InputFloat( "##gravityScale", &physicsAttributes.gravityScale, 1.f, 1.f, "%.1f" );
			ImGui::SetItemTooltip( "Scale the gravity applied to this body. Non-dimensional." );

			ui::PropertyLabel( "Box Shape" );
			if ( ui::ToggleSwitch( "##boxShape", &physicsAttributes.boxShape ) )
			{
				if ( physicsAttributes.circle )
					physicsAttributes.circle = false;
			}
			ImGui::SetItemTooltip( "If the entity is a box, the box collider is used in construction for the size." );

			if ( physicsAttributes.boxShape && !physicsAttributes.circle )
			{
				ui::PropertyLabel( "" );
				ImGui::TextColored( ImVec4{ 1.f, 1.f, 0.f, 1.f }, "Must have a Box Collider Component." );
			}

			ui::PropertyLabel( "Circle Shape" );
			if ( ui::ToggleSwitch( "##circle", &physicsAttributes.circle ) )
			{
				if ( physicsAttributes.boxShape )
					physicsAttributes.boxShape = false;
			}
			ImGui::SetItemTooltip(
				"If the entity is a circle, the circle collider is used in construction for the size." );

			if ( !physicsAttributes.boxShape && physicsAttributes.circle )
			{
				ui::PropertyLabel( "" );
				ImGui::TextColored( ImVec4{ 1.f, 1.f, 0.f, 1.f }, "Must have a Circle Collider Component." );
			}

			ui::PropertyLabel( "Fixed Rotation" );
			ui::ToggleSwitch( "##fixedRotation", &physicsAttributes.fixedRotation );
			ImGui::SetItemTooltip( "Should the body be prevented from rotating?" );

			ui::PropertyLabel( "Sensor" );
			ui::ToggleSwitch( "##sensor", &physicsAttributes.isSensor );
			ImGui::SetItemTooltip(
				"A sensor shape generates overlap events but never generates a collision response.\n"
				"Sensors do not collide with other sensors and do not have continuous collision." );

			ui::EndPropertyTable();
		}

		ImGui::SeparatorText( "Physics Object Data" );
		if ( ui::BeginPropertyTable( "##PhysicsObjectDataProps" ) )
		{
			auto& objectData = physicsAttributes.objectData;

			DrawTextProperty( "Tag", "##_tag", objectData.tag );
			DrawTextProperty( "Group", "##_group", objectData.group );

			ui::PropertyLabel( "Collider" );
			ui::ToggleSwitch( "##objectDataCollider", &objectData.isCollider );

			ui::PropertyLabel( "Trigger" );
			ui::ToggleSwitch( "##objectDataTrigger", &objectData.isTrigger );

			ui::PropertyLabel( "Friendly" );
			ui::ToggleSwitch( "##objectDataFriendly", &objectData.isFriendly );

			ui::EndPropertyTable();
		}
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
