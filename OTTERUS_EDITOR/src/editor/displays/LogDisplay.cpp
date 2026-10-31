#include "LogDisplay.h"
#include <Logger/Logger.h>
#include <ranges>
#include <format>

#include "imgui_internal.h"

namespace otterus_editor {

	LogDisplay::LogDisplay()
		: m_AutoScroll{ true }
	{
	}

	void LogDisplay::Clear()
	{
		m_Logs.clear();
		m_FilteredIndices.clear();
		m_SelectedLine = -1;
		m_InfoCount = m_WarnCount = m_ErrorCount = 0;
	}

	void LogDisplay::UpdateFilter()
	{
		m_FilteredIndices.clear();
		for (int i = 0; i < static_cast<int>(m_Logs.size()); ++i)
		{
			const auto& entry = m_Logs[i];
			if ((entry.type == otterus_logger::LogType::INFO && m_ShowInfo) ||
				(entry.type == otterus_logger::LogType::WARN && m_ShowWarn) ||
				(entry.type == otterus_logger::LogType::ERR  && m_ShowError))
			{
				if (m_Filter.IsActive() && !m_Filter.PassFilter(entry.log.c_str()))
				{
					continue;
				}

				m_FilteredIndices.push_back(i);
			}
		}
	}

	void LogDisplay::GetLogs()
	{
		if (OTTERUS_LOG_ADDED())
		{
			Clear();
			const auto& engineLogs = OTTERUS_GET_LOGS();
			m_Logs.assign(engineLogs.rbegin(), engineLogs.rend());

			m_InfoCount = m_WarnCount = m_ErrorCount = 0;
			for (const auto& log : m_Logs)
			{
				if (log.type == otterus_logger::LogType::INFO) m_InfoCount++;
				else if (log.type == otterus_logger::LogType::WARN) m_WarnCount++;
				else if (log.type == otterus_logger::LogType::ERR) m_ErrorCount++;
			}

			UpdateFilter();
			OTTERUS_RESET_ADDED();
		}
	}

	void LogDisplay::DrawToolbar()
	{
		if (ImGui::Button("Clear"))
		{
			Clear();
			OTTERUS_CLEAR_LOGS();
		}
		ImGui::SameLine();
		if (ImGui::Button("Copy All"))
		{
			std::string allLogs;
			for (const auto& entry : m_Logs)
			{
				allLogs += std::format("[{0}] [{1}] {2} - {3}\n", 
					entry.timestamp, entry.origin, entry.log, entry.stackTrace.empty() ? "" : ("\n" + entry.stackTrace));
			}
			ImGui::SetClipboardText(allLogs.c_str());
		}
		ImGui::SameLine();
		ImGui::Checkbox("Auto-scroll", &m_AutoScroll);

		auto FilterButton = [this](const char* label, bool& state, const ImVec4& activeColor) {
			if (state)
				ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
			else
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

			if (ImGui::Button(label))
			{
				state = !state;
				UpdateFilter();
			}
			ImGui::PopStyleColor();
		};

		ImGui::SameLine();
		FilterButton(std::format("Info ({0})###filter_info", m_InfoCount).c_str(), m_ShowInfo, ImVec4(0.2f, 0.5f, 0.2f, 1.f));
		ImGui::SameLine();
		FilterButton(std::format("Warn ({0})###filter_warn", m_WarnCount).c_str(), m_ShowWarn, ImVec4(0.6f, 0.5f, 0.1f, 1.f));
		ImGui::SameLine();
		FilterButton(std::format("Error ({0})###filter_err", m_ErrorCount).c_str(), m_ShowError, ImVec4(0.6f, 0.2f, 0.2f, 1.f));

		ImGui::SameLine();
		ImGui::SetNextItemWidth(200.0f);
		if (ImGui::InputTextWithHint("##LogSearch", "Search...", m_Filter.InputBuf, IM_COUNTOF(m_Filter.InputBuf)))
		{
			m_Filter.Build();
			UpdateFilter();
		}
		if (m_Filter.IsActive())
		{
			ImGui::SameLine();
			if (ImGui::SmallButton("x##clear_search"))
			{
				m_Filter.Clear();
				UpdateFilter();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Clear search");
			}
		}
	}

	void LogDisplay::DrawLogList(float reservedHeight)
	{
		ImGui::BeginChild("LogList", ImVec2{ 0.f, -reservedHeight }, false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 2.f });
		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(m_FilteredIndices.size()));

		while (clipper.Step()) {
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				int logIndex = m_FilteredIndices[i];
				const auto& entry = m_Logs[logIndex];

				ImVec4 color{ 1.f, 1.f, 1.f, 1.f };
				const char* typeTag = "[INFO]";
				switch (entry.type)
				{
				case otterus_logger::LogType::INFO:
					color = ImVec4{ 0.2f, 1.0f, 0.4f, 1.f };
					typeTag = "[INFO]";
					break;
				case otterus_logger::LogType::WARN:
					color = ImVec4{ 1.0f, 0.85f, 0.2f, 1.f };
					typeTag = "[WARN]";
					break;
				case otterus_logger::LogType::ERR:
					color = ImVec4{ 1.0f, 0.3f, 0.3f, 1.f };
					typeTag = "[ERROR]";
					break;
				default:
					break;
				}

				ImGui::PushID(logIndex);
				ImGui::PushStyleColor(ImGuiCol_Text, color);

				bool isSelected = (m_SelectedLine == logIndex);
				std::string rowLabel = std::format("{0} {1} - {2}", typeTag, entry.timestamp, entry.log);

				if (ImGui::Selectable(rowLabel.c_str(), isSelected, ImGuiSelectableFlags_SpanAvailWidth))
				{
					m_SelectedLine = logIndex;
				}

				ImGui::PopStyleColor();

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Copy Message"))
					{
						ImGui::SetClipboardText(entry.log.c_str());
					}
					if (ImGui::MenuItem("Copy Details"))
					{
						std::string details = std::format("Message: {0}\nTime: {1}\nOrigin: {2}\n{3}",
							entry.log, entry.timestamp, entry.origin, entry.stackTrace);
						ImGui::SetClipboardText(details.c_str());
					}
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}
		}

		clipper.End();
		ImGui::PopStyleVar();

		if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.f);
		}

		ImGui::EndChild();
	}

	void LogDisplay::DrawDetailsPane()
	{
		ImGui::BeginChild("LogDetails", ImVec2{ 0.f, 0.f }, false, ImGuiWindowFlags_HorizontalScrollbar);

		if (m_SelectedLine >= 0 && m_SelectedLine < static_cast<int>(m_Logs.size()))
		{
			const auto& selected = m_Logs[m_SelectedLine];

			if (ImGui::SmallButton("Copy Full Details"))
			{
				std::string details = std::format("Message: {0}\nTime: {1}\nOrigin: {2}\n{3}",
					selected.log, selected.timestamp, selected.origin, selected.stackTrace);
				ImGui::SetClipboardText(details.c_str());
			}
			ImGui::SameLine();
			ImGui::TextDisabled("| Log Inspector");

			ImGui::Separator();
			ImGui::TextWrapped("%s", selected.log.c_str());
			ImGui::Spacing();
			ImGui::TextDisabled("Origin: %s", selected.origin.empty() ? "N/A" : selected.origin.c_str());
			ImGui::TextDisabled("Time:   %s", selected.timestamp.c_str());

			if (!selected.stackTrace.empty())
			{
				ImGui::Spacing();
				ImGui::TextColored(ImVec4{ 1.0f, 0.35f, 0.35f, 1.0f }, "Stack Trace:");
				ImGui::Separator();
				ImGui::TextUnformatted(selected.stackTrace.c_str());
			}
		}
		else
		{
			ImGui::TextDisabled("Select a log row above to view its origin and details.");
		}

		ImGui::EndChild();
	}

	void LogDisplay::Draw()
	{
		if (!ImGui::Begin("Console"))
		{
			ImGui::End();
			return;
		}

		GetLogs();
		DrawToolbar();
		ImGui::Separator();
		DrawLogList(150.0f);
		ImGui::Separator();
		DrawDetailsPane();

		ImGui::End();
	}
}
