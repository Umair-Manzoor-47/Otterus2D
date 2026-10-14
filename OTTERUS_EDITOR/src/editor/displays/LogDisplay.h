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
		int m_SelectedLine{ -1 };

	private:
		void GetLogs();

	public:
		LogDisplay();
		~LogDisplay() = default;

		void Clear();

		void Draw() override;
		inline int GetSelectedLine() const { return m_SelectedLine; };

	};
}