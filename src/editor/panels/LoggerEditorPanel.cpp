#include "LoggerEditorPanel.h"

#include "ImGuiHelpers.h"
#include "Logger.h"
#include "imgui.h"

void LoggerEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str(), nullptr, ImGuiWindowFlags_MenuBar);
  DrawMenuBar();
  DrawLogArea();
  ImGui::End();
}

void LoggerEditorPanel::DrawMenuBar() {
  static bool shouldOpenPopup = false;

  if (ImGui::BeginMenuBar()) {
    if (ImGui::MenuItem("Clear Buffer")) {
      m_Logger.ClearLog();
    }
    ImGui::EndMenuBar();
  }
}

void LoggerEditorPanel::DrawLogArea() {
  // scroll log area
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
  ImGui::BeginChild("LogScrollArea", ImVec2(0, 0), true,
                    ImGuiWindowFlags_AlwaysVerticalScrollbar);

  for (const auto &log : m_Logger.GetLogs()) {
    ImVec4 sevColor;
    switch (log.severity) {
    case INFO:
      sevColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
      break; // Green
    case WARNING:
      sevColor = ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
      break; // Yellow
    case ERROR:
      sevColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
      break; // red
    }

    // severity
    ImGui::PushStyleColor(ImGuiCol_Text, sevColor);
    ImGui::Text("[%s]", Logger::GetSevText(log.severity));
    ImGui::PopStyleColor();

    // file and line
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.7f, 1.0f, 1.0f));
    ImGui::Text("%s:%d", log.file.c_str(), log.line);
    ImGui::PopStyleColor();

    // function
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.9f, 0.9f, 1.0f));
    ImGui::Text("%s", log.function.c_str());
    ImGui::PopStyleColor();

    // message
    ImGui::Text("%s", log.message.c_str());
  }

  ImGui::EndChild();
  ImGui::PopStyleColor();
}
