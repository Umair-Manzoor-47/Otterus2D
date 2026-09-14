#include "RendererBindings.h"
#include <Rendering/Essentials/Primitives.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Core/Renderer.h>
#include "../ECS/Registry.h"
#include <Logger/Logger.h>
#include "../Resources/AssetManager.h"

using namespace otterus_rendering;

void otterus_core::Scripting::RendererBinder::CreateRendererBind(sol::state& lua, otterus_core::ECS::Registry& registry)
{
	auto& assetManager = registry.GetContext<std::shared_ptr<otterus_resources::AssetManager>>();

	lua.new_usertype<Line>(
		"Line",
		sol::call_constructor,
		sol::factories(
			[](const glm::vec2& p1, glm::vec2& p2, const Color& color) {

				return Line{
					.p1 = p1,
					.p2 = p2,
					.color = color,
				};
			}
		),
		"p1", &Line::p1,
		"p2", &Line::p2,
		"color", &Line::color,
		"line_width", &Line::lineWidth
	);


	lua.new_usertype<Rect>(
		"Rect",
		sol::call_constructor,
		sol::factories(
			[](const glm::vec2& position, float height, float width, const Color& color) {

				return Rect{
					.position = position,
					.width = width,
					.height = height,
					.color = color,
				};
			}
		),
		"position", &Rect::position,
		"width", &Rect::width,
		"height", &Rect::height,
		"color", &Rect::color
	);

	lua.new_usertype<Circle>(
		"Circle",
		sol::call_constructor,
		sol::factories(
			[](const glm::vec2& position, float lineThickness, float radius, const Color& color)
			{
				return Circle{ .position = position, .lineThickness = lineThickness, .radius = radius, .color = color };
			}
		),
		"position", &Circle::position,
		"lineThickness", &Circle::lineThickness,
		"radius", &Circle::radius,
		"color", &Circle::color
	);

	lua.new_usertype<Text>(
		"Text",
		sol::call_constructor,
		sol::factories(
			[&](const glm::vec2& position, const std::string& textStr, const std::string& fontName, float wrap, const Color& color)
			{
				auto font = assetManager->GetFont(fontName);
				if (!font)
				{
					OTTERUS_ERROR("Failed to get font [{}] in Asset Manager.", fontName);
					return Text{};
				}
				return Text{
					.position = position,
					.textStr = textStr,
					.wrap = wrap,
					.font = font,
					.color = color
				};

			}
		),
		"position", &Text::position,
		"textStr", &Text::textStr,
		"wrap", &Text::wrap,
		"color", &Text::color
	);

	auto& renderer = registry.GetContext<std::shared_ptr<Renderer>>();

	if (!renderer) {
		OTTERUS_ERROR("Failed to get the Renderer from Registry.");
		return;
	}

	lua.set_function(
		"DrawCircle", sol::overload(
			[&](const Circle& circle){
			
				renderer->DrawCircle(circle);
			},
			[&](const glm::vec2& position, float lineThickness, float radius, const Color& color) {

				renderer->DrawCircle(position, radius, color, lineThickness);
			}
		)
	);

	lua.set_function(
		"DrawFilledRect", [&](const Rect& rect) {
			renderer->DrawFillRect(rect);
		}
	);


	lua.set_function(
		"DrawRect", sol::overload(
			[&](const Rect& rect) {
				renderer->DrawRect(rect);
			},
			[&](const glm::vec2& position, float width, float height, const Color& color) {
				renderer->DrawRect(
					Rect{
						.position = position,
						.width = width,
						.height = height,
						.color = color
					}
				);
			}

		)
	);
	lua.set_function(
		"DrawLine", sol::overload(
			[&](const Line& line) {
				renderer->DrawLine(line);
			},
			[&](const glm::vec2& p1, const glm::vec2& p2, const Color& color) {
				renderer->DrawLine(
					Line{
						.p1 = p1,
						.p2 = p2,
						.color = color,
					}
				);
			}

		)
	);

	lua.set_function(
		"DrawText",	[&](const Text& text) {
				renderer->DrawText2D(text);
			}
	);

	auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();
	if (!camera) {
		OTTERUS_ERROR("Camera binding failed - Error while getting the Camera2D from Registry.");
		return;
	}

	// Camera2D Binding
	lua.new_usertype<Camera2D>(
		"Camera",
		sol::no_constructor,
		"get", [&] {
			return *camera;
		},
		"position", [&] {
			return camera->GetPosition();
		},
		"set_position", [&](const glm::vec2& newPosition) {
			camera->SetPosition(newPosition);
		},
		"scale", [&] {
			return camera->GetScale();
		},
		"set_scale", [&] (float newScale) {
			return camera->SetScale(newScale);
		},
		"get_world_coords", [&](glm::vec2 screenCoords) {
			return camera->ScreenCoordToWorld(screenCoords);
		},
		"get_screen_coords", [&](glm::vec2 worldCoords) {
			return camera->WorldCoordToScreen(worldCoords);
		},
		"width", [&] {
			return camera->GetWidth();
		},
		"height", [&] {
			return camera->GetHeight();
		}
	
	);
}
