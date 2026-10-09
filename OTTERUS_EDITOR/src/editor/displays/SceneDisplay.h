#pragma once
#include <Core/ECS/Registry.h>
#include "IDisplay.h"

namespace otterus_editor {
	class SceneDisplay : public IDisplay
	{
	private:
		otterus_core::ECS::Registry& m_Registry;
		bool m_PlayScene, m_SceneLoaded;

	private:
		void LoadScene();
		void UnloadScene();

	public:
		SceneDisplay(otterus_core::ECS::Registry& registry);
		~SceneDisplay() = default;


		virtual void Draw() override;
		virtual void Update() override;

	};

}