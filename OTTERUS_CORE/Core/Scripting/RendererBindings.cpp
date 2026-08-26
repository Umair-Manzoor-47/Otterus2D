#include "RendererBindings.h"
#include <Rendering/Essentials/Primitives.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Core/Renderer.h>
#include "../ECS/Registry.h"
#include <Logger/Logger.h>

using namespace otterus_rendering;

void otterus_core::Scripting::RendererBinder::CreateRendererBind(sol::state& lua, otterus_core::ECS::Registry& registry)
{
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
			[](const glm::vec2& position, float thickness, float radius, const Color color) {

				return Circle{
					.position = position,
					.radius = radius,
					.color = color,
				};
			}
		),
		"position", &Circle::position,
		"radius",	&Circle::radius,
		"color",	&Circle::color
	);

	auto& renderer = registry.GetContext<std::shared_ptr<Renderer>>();

	if (!renderer) {
		OTTERUS_ERROR("Failed to get the Renderer from Registry.");
		return;
	}

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
		}
	
	);
}
