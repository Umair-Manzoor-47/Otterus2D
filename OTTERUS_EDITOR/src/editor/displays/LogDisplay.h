#pragma once
#include <imgui.h>
#include "IDisplay.h"
#include <Logger.h>
#include <vector>

namespace otterus_editor {

	class LogDisplay : public IDisplay
	{
	private:
		std::vector<otterus_logger::LogEntry> m_Logs;
		std::vector<int> m_FilteredIndices;

		bool m_AutoScroll{ true };
		bool m_ShowInfo{ true };
		bool m_ShowWarn{ true };
		bool m_ShowError{ true };

		size_t m_InfoCount{ 0 };
		size_t m_WarnCount{ 0 };
		size_t m_ErrorCount{ 0 };

		int m_SelectedLine{ -1 };

		ImGuiTextFilter m_Filter;

	private:
		void GetLogs();
		void UpdateFilter();
		void DrawToolbar();
		void DrawLogList(float reservedHeight);
		void DrawDetailsPane();

	public:
		LogDisplay();
		~LogDisplay() = default;

		void Clear();
		void Draw() override;
		int GetSelectedLine() const { return m_SelectedLine; }
	};
}