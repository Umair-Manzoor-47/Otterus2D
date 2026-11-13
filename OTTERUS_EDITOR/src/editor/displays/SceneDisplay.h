#pragma once
#include <Core/ECS/Registry.h>
#include "IDisplay.h"

namespace otterus_editor {
	class SceneDisplay : public IDisplay
	{
	private:
		bool m_PlayScene, m_SceneLoaded;

	private:
		void LoadScene();
		void UnloadScene();

	public:
		SceneDisplay();
		~SceneDisplay() = default;


		virtual void Draw() override;
		void RenderScene();
		virtual void Update() override;

	};

}