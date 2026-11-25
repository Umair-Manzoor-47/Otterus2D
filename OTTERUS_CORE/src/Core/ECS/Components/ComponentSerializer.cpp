#include "Core/ECS/Components/ComponentSerializer.h"

#include "CoreUtilities.h"
#include "OtterusFileSystem/Serializers/JSONSerializer.h"

namespace otterus_core::ECS
{
    void ComponentSerializer::SerializeComponent(otterus_filesystem::JSONSerializer& serializer,
        const TransformComponent& transform)
    {
            serializer.StartNewObject( "transform" )
        .StartNewObject( "position" )
        .AddKeyValuePair( "x", transform.position.x )
        .AddKeyValuePair( "y", transform.position.y )
        .EndObject()
        .StartNewObject( "scale" )
        .AddKeyValuePair( "x", transform.scale.x )
        .AddKeyValuePair( "y", transform.scale.y )
        .EndObject()
        .AddKeyValuePair( "rotation", transform.rotation )
        .EndObject();
    }

    void ComponentSerializer::SerializeComponent(otterus_filesystem::JSONSerializer& serializer,
        const SpriteComponent& sprite)
    {
            serializer.StartNewObject( "sprite" )
        .AddKeyValuePair( "width", sprite.width )
        .AddKeyValuePair( "height", sprite.height )
        .AddKeyValuePair( "start_x", sprite.start_x )
        .AddKeyValuePair( "start_y", sprite.start_y )
        .AddKeyValuePair( "layer", sprite.layer )
        .AddKeyValuePair( "texture_name", sprite.texture_name )
        .StartNewObject( "uvs" )
        .AddKeyValuePair( "u", sprite.uvs.u )
        .AddKeyValuePair( "v", sprite.uvs.v )
        .AddKeyValuePair( "uv_width", sprite.uvs.uv_width )
        .AddKeyValuePair( "uv_height", sprite.uvs.uv_height )
        .EndObject()
        .StartNewObject( "color" )
        .AddKeyValuePair( "r", sprite.color.r )
        .AddKeyValuePair( "g", sprite.color.g )
        .AddKeyValuePair( "b", sprite.color.b )
        .AddKeyValuePair( "a", sprite.color.a )
        .EndObject()
        .AddKeyValuePair( "isHidden", sprite.isHidden )
        .EndObject();
    }

	void ComponentSerializer::SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
												  const AnimationComponent& animation )
	{
		serializer.StartNewObject( "animation" )
			.AddKeyValuePair( "numFrames", animation.numFrames )
			.AddKeyValuePair( "frameRate", animation.frameRate )
			.AddKeyValuePair( "frameOffset", animation.frameOffset )
			.AddKeyValuePair( "vertical", animation.vertical )
			.AddKeyValuePair( "looped", animation.looped )
			.EndObject();
	}

	void ComponentSerializer::SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
												  const BoxColliderComponent& boxCollider )
	{
		serializer.StartNewObject( "boxCollider" )
			.AddKeyValuePair( "width", boxCollider.width )
			.AddKeyValuePair( "height", boxCollider.height )
			.StartNewObject( "offset" )
			.AddKeyValuePair( "x", boxCollider.offset.x )
			.AddKeyValuePair( "y", boxCollider.offset.y )
			.EndObject()
			.EndObject();
	}

	void ComponentSerializer::SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
												  const CircleColliderComponent& circleCollider )
	{
		serializer.StartNewObject( "circleCollider" )
			.AddKeyValuePair( "radius", circleCollider.radius )
			.StartNewObject( "offset" )
			.AddKeyValuePair( "x", circleCollider.offset.x )
			.AddKeyValuePair( "y", circleCollider.offset.y )
			.EndObject()
			.EndObject();
	}

	void ComponentSerializer::SerializeComponent( otterus_filesystem::JSONSerializer& serializer, const TextComponent& text )
	{
		serializer
			.StartNewObject( "text" ) // Start text table
			.AddKeyValuePair( "textStr", text.textStr )
			.AddKeyValuePair( "fontName", text.fontName )
			.AddKeyValuePair( "padding", text.padding )
			.AddKeyValuePair( "wrap", text.wrap )
			.AddKeyValuePair( "isHidden", text.isHidden )
			.StartNewObject( "color" ) // Start Color table
			.AddKeyValuePair( "r", static_cast<int>( text.color.r ) )
			.AddKeyValuePair( "g", static_cast<int>( text.color.g ) )
			.AddKeyValuePair( "b", static_cast<int>( text.color.b ) )
			.AddKeyValuePair( "a", static_cast<int>( text.color.a ) )
			.EndObject();		// End Color table
		serializer.EndObject(); // End Text table
	}

	void ComponentSerializer::SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
												  const PhysicsComponent& physics )
	{
		const auto& attributes = physics.GetAttributes();

		serializer.StartNewObject( "physics" )
			.StartNewObject( "attributes" )
			.AddKeyValuePair( "type", GetRigidBodyTypeString( attributes.type ) )
			.AddKeyValuePair( "density", attributes.density )
			.AddKeyValuePair( "friction", attributes.friction )
			.AddKeyValuePair( "restitution", attributes.restitution )
			.AddKeyValuePair( "restitutionThreshold", attributes.restitutionThreshold )
			.AddKeyValuePair( "radius", attributes.radius )
			.AddKeyValuePair( "gravityScale", attributes.gravityScale )
			.StartNewObject( "position" )
			.AddKeyValuePair( "x", attributes.position.x )
			.AddKeyValuePair( "y", attributes.position.y )
			.EndObject()
			.StartNewObject( "scale" )
			.AddKeyValuePair( "x", attributes.scale.x )
			.AddKeyValuePair( "y", attributes.scale.y )
			.EndObject()
			.StartNewObject( "boxSize" )
			.AddKeyValuePair( "x", attributes.boxSize.x )
			.AddKeyValuePair( "y", attributes.boxSize.y )
			.EndObject()
			.StartNewObject( "offset" )
			.AddKeyValuePair( "x", attributes.offset.x )
			.AddKeyValuePair( "y", attributes.offset.y )
			.EndObject()
			.AddKeyValuePair( "circle", attributes.circle )
			.AddKeyValuePair( "boxShape", attributes.boxShape )
			.AddKeyValuePair( "fixedRotation", attributes.fixedRotation )
			.AddKeyValuePair( "isSensor", attributes.isSensor )
			.AddKeyValuePair( "filterCategory", static_cast<unsigned>( attributes.filterCategory ) )
			.AddKeyValuePair( "filterMask", static_cast<unsigned>( attributes.filterMask ) )
			.AddKeyValuePair( "groupIndex", static_cast<int>( attributes.groupIndex ) )
			.StartNewObject( "objectData" )
			.AddKeyValuePair( "tag", attributes.objectData.tag )
			.AddKeyValuePair( "group", attributes.objectData.group )
			.AddKeyValuePair( "isCollider", attributes.objectData.isCollider )
			.AddKeyValuePair( "isTrigger", attributes.objectData.isTrigger )
			.AddKeyValuePair( "isFriendly", attributes.objectData.isTrigger )
			.EndObject()
			.EndObject()
			.EndObject();
	}

	void ComponentSerializer::SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
												  const RigidBodyComponent& rigidBody )
	{
		serializer
			.StartNewObject( "rigidBody" ) // Start rigidbody table
			.StartNewObject( "velocity" )  // Start velocity table
			.AddKeyValuePair( "x", rigidBody.velocity.x )
			.AddKeyValuePair( "y", rigidBody.velocity.y )
			.EndObject()  // end velocity table
			.EndObject(); // End rigid body table
	}

	void ComponentSerializer::SerializeComponent( otterus_filesystem::JSONSerializer& serializer,
												  const LightComponent& light )
	{
		serializer.StartNewObject( "light" )
			.StartNewObject( "color" )
			.AddKeyValuePair( "r", static_cast<int>( light.color.r ) )
			.AddKeyValuePair( "g", static_cast<int>( light.color.g ) )
			.AddKeyValuePair( "b", static_cast<int>( light.color.b ) )
			.AddKeyValuePair( "a", static_cast<int>( light.color.a ) )
			.EndObject()
			.StartNewObject( "offset" )
			.AddKeyValuePair( "x", light.offset.x )
			.AddKeyValuePair( "y", light.offset.y )
			.EndObject()
			.AddKeyValuePair( "intensity", light.intensity )
			.AddKeyValuePair( "radius", light.radius )
			.AddKeyValuePair( "falloff", light.falloff )
			.AddKeyValuePair( "isSpotlight", light.isSpotlight )
			.AddKeyValuePair( "innerAngle", light.innerAngle )
			.AddKeyValuePair( "outerAngle", light.outerAngle )
			.AddKeyValuePair( "castShadows", light.castShadows )
			.EndObject();
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, TransformComponent& transform )
	{
		transform.position =
			glm::vec2{ jsonValue[ "position" ][ "x" ].GetFloat(), jsonValue[ "position" ][ "y" ].GetFloat() };

		transform.scale = glm::vec2{ jsonValue[ "scale" ][ "x" ].GetFloat(), jsonValue[ "scale" ][ "y" ].GetFloat() };
		transform.rotation = jsonValue[ "rotation" ].GetFloat();
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, SpriteComponent& sprite )
	{
		sprite.width = jsonValue[ "width" ].GetFloat();
		sprite.height = jsonValue[ "height" ].GetFloat();

		sprite.uvs = UVs{ .u = jsonValue[ "uvs" ][ "u" ].GetFloat(),
						  .v = jsonValue[ "uvs" ][ "v" ].GetFloat(),
						  .uv_width = jsonValue[ "uvs" ][ "uv_width" ].GetFloat(),
						  .uv_height = jsonValue[ "uvs" ][ "uv_height" ].GetFloat() };

		sprite.start_x = jsonValue[ "start_x" ].GetInt();
		sprite.start_y = jsonValue[ "start_y" ].GetInt();
		sprite.layer = jsonValue[ "layer" ].GetInt(), sprite.isHidden = jsonValue[ "isHidden" ].GetBool();
		sprite.texture_name = jsonValue[ "texture_name" ].GetString();
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, AnimationComponent& animation )
	{
		animation.numFrames = jsonValue[ "numFrames" ].GetInt();
		animation.frameRate = jsonValue[ "frameRate" ].GetInt();
		animation.frameOffset = jsonValue[ "frameOffset" ].GetInt();
		animation.vertical = jsonValue[ "vertical" ].GetBool();
		animation.looped = jsonValue[ "looped" ].GetBool();
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, BoxColliderComponent& boxCollider )
	{
		boxCollider.width = jsonValue[ "width" ].GetInt();
		boxCollider.height = jsonValue[ "height" ].GetInt();
		boxCollider.offset = glm::vec2{ jsonValue[ "offset" ][ "x" ].GetFloat(), jsonValue[ "offset" ][ "y" ].GetFloat() };
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue,
													CircleColliderComponent& circleCollider )
	{
		circleCollider.radius = jsonValue[ "radius" ].GetFloat();
		circleCollider.offset =
			glm::vec2{ jsonValue[ "offset" ][ "x" ].GetFloat(), jsonValue[ "offset" ][ "y" ].GetFloat() };
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, TextComponent& text )
	{
		text.textStr = jsonValue[ "textStr" ].GetString();
		text.fontName = jsonValue[ "fontName" ].GetString();
		text.padding = jsonValue[ "padding" ].GetInt();
		text.wrap = jsonValue[ "wrap" ].GetInt();
		text.isHidden = jsonValue[ "isHidden" ].GetBool();

		text.color = otterus_rendering::Color{ .r = static_cast<GLubyte>( jsonValue[ "color" ][ "r" ].GetInt() ),
											 .g = static_cast<GLubyte>( jsonValue[ "color" ][ "g" ].GetInt() ),
											 .b = static_cast<GLubyte>( jsonValue[ "color" ][ "b" ].GetInt() ),
											 .a = static_cast<GLubyte>( jsonValue[ "color" ][ "a" ].GetInt() ) };
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, PhysicsComponent& physics )
	{
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, RigidBodyComponent& rigidBody )
	{
		rigidBody.velocity.x = jsonValue[ "velocity" ][ "x" ].GetFloat();
		rigidBody.velocity.y = jsonValue[ "velocity" ][ "y" ].GetFloat();
	}

	void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, LightComponent& light )
	{
		const auto& color = jsonValue[ "color" ];
		light.color.r = static_cast<GLubyte>( color[ "r" ].GetInt() );
		light.color.g = static_cast<GLubyte>( color[ "g" ].GetInt() );
		light.color.b = static_cast<GLubyte>( color[ "b" ].GetInt() );
		light.color.a = static_cast<GLubyte>( color[ "a" ].GetInt() );

		if ( jsonValue.HasMember( "offset" ) )
		{
			const auto& offset = jsonValue[ "offset" ];
			light.offset = glm::vec2{ offset[ "x" ].GetFloat(), offset[ "y" ].GetFloat() };
		}

		light.intensity = jsonValue[ "intensity" ].GetFloat();
		light.radius = jsonValue[ "radius" ].GetFloat();
		light.falloff = jsonValue[ "falloff" ].GetFloat();
		light.isSpotlight = jsonValue[ "isSpotlight" ].GetBool();
		light.innerAngle = jsonValue[ "innerAngle" ].GetFloat();
		light.outerAngle = jsonValue[ "outerAngle" ].GetFloat();
		light.castShadows = jsonValue[ "castShadows" ].GetBool();
	}
}
