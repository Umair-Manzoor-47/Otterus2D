#include "LogDisplay.h"
#include <Logger/Logger.h>
#include <ranges>

namespace otterus_editor {

	void LogDisplay::GetLogs()
	{
		if (OTTERUS_LOG_ADDED())
		{
			Clear();
			std::ranges::reverse_view rLogs{
				OTTERUS_GET_LOGS()
			};

			for (const auto& log : rLogs)
			{
				int oldTextSize = m_TextBuffer.size();
				m_TextBuffer.append(log.log.c_str());
				m_TextOffsets.push_back(oldTextSize + 1);
			}

			OTTERUS_RESET_ADDED();
		}
	}

	LogDisplay::LogDisplay()
		: m_AutoScroll{ true }
	{
	}

	void LogDisplay::Clear()
	{
		m_TextBuffer.clear();
		m_TextOffsets.clear();
	}

	void LogDisplay::Draw()
	{
		if (!ImGui::Begin("Logs"))
		{
			ImGui::End();
			return;
		}

		GetLogs();

		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			Clear();
			OTTERUS_CLEAR_LOGS();
		}
		ImGui::SameLine();
		if (ImGui::Button("Copy"))
		{
			ImGui::LogToClipboard();
		}
		ImGui::SameLine();
		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2{ 0.f, 0.f }, false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
		ImGuiListClipper clipper;
		clipper.Begin(m_TextOffsets.Size);

		while (clipper.Step()) {

			for (int lineNo = clipper.DisplayStart; lineNo < clipper.DisplayEnd; lineNo++)
			{
				const char* line_start = m_TextBuffer.begin() + m_TextOffsets[lineNo] - 1;
				const char* line_end = ((lineNo + 1) < m_TextOffsets.Size) 
											? m_TextBuffer.begin() + m_TextOffsets[lineNo + 1] - 1
											: m_TextBuffer.end();
				std::string text{line_start, line_end};

				ImVec4 color{ 1.f, 1.f, 1.f, 1.f };
				if (text.find("INFO") != std::string::npos)
					color = ImVec4{ 0.f, 1.f, 0.f, 1.f };
				if (text.find("ERROR") != std::string::npos)
					color = ImVec4{ 1.f, 0.f, 0.f, 1.f };
				if (text.find("WARN") != std::string::npos)
					color = ImVec4{ 1.f, 1.f, 0.f, 1.f };

				ImGui::PushStyleColor( ImGuiCol_Text, color );
				ImGui::TextUnformatted( line_start, line_end );
				ImGui::PopStyleColor();
			}
		}
		
		clipper.End();
		ImGui::PopStyleVar();

		if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.f);
		}

		ImGui::EndChild();
		ImGui::End();
	}
}