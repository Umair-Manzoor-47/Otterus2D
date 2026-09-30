#pragma once
#include <imgui.h>
#include "IDisplay.h"

namespace otterus_editor {

	class LogDisplay : public IDisplay
	{
	private:
		ImGuiTextBuffer m_TextBuffer;
		ImVector<int> m_TextOffsets;

		bool m_AutoScroll;

	private:
		void GetLogs();

	public:
		LogDisplay();
		~LogDisplay() = default;

		void Clear();

		virtual void Draw() override;

	};
}