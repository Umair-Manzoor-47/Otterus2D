#pragma once
#include <Core/ECS/Registry.h>

namespace otterus_editor {
	class SceneDisplay
	{
	private:
		otterus_core::ECS::Registry& m_Registry;
	public:
		SceneDisplay(otterus_core::ECS::Registry& registry);
		~SceneDisplay() = default;

		void Draw();

	};

}