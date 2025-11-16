#include "LoggerEditorPanel.h"

#include "Logger.h"
#include "imgui.h"

void LoggerEditorPanel::OnImGuiRender(){
  if(!m_IsOpen) return;

  auto console = Logger::Get();

  ImGui::Begin(m_Name.c_str());

  if(ImGui::BeginMenuBar()){
    ImGui::EndMenuBar();
  }
  
  // scroll log area
  ImGui::BeginChild("LogScrollArea", ImVec2(0,0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

  for(const auto& log : console.GetLogs()){
    const std::string text =  console.GetLogFormattedText(log);
    const char* ctext = text.c_str();
    ImGui::Text(ctext); 
  }

  ImGui::PopStyleColor();
  ImGui::EndChild();
  ImGui::End();
}
