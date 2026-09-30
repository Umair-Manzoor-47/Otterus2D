#pragma once
#include <Core/ECS/Registry.h>
#include "IDisplay.h"

namespace otterus_editor {
	class SceneDisplay : public IDisplay
	{
	private:
		otterus_core::ECS::Registry& m_Registry;
	public:
		SceneDisplay(otterus_core::ECS::Registry& registry);
		~SceneDisplay() = default;

		virtual void Draw() override;

	};

}