#include "FileSystemEditorPanel.h"

#include "imgui.h"

#include "FileSystem.h"

void FileSystemEditorPanel::OnImGuiRender() {
  if(!m_IsOpen)
    return;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);
     
  for (int i = 0 ; i < m_Files.size(); i++) {
    const auto &file = m_Files[i];
    bool isSelected = m_SelectedIndex == i;
    std::string uniqueName = std::string(file.stem) + file.extension + "##" +
                             std::to_string(i);
    std::string popupId = "FileContext##" + std::to_string(i);
      
      // show file selectable
    if (ImGui::Selectable(uniqueName.c_str(), isSelected,
                          ImGuiSelectableFlags_AllowDoubleClick |
                          ImGuiSelectableFlags_SelectOnNav)) {
     m_SelectedIndex = i;
    }
    if(ImGui::IsItemHovered()){
      m_SelectedIndex = i;
    }

    // // allow context menu
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)){
      ImVec2 mousePos = ImGui::GetMousePos();
      mousePos.y -= 20;
      ImGui::SetNextWindowPos(mousePos);
      ImGui::OpenPopup(popupId.c_str());
    }
    if (ImGui::BeginPopup(popupId.c_str())) {
      ImGui::Text("File: %s", file.getFullName().c_str());
      OpenContextMenu();
      ImGui::EndPopup();
    }
  }


  ImGui::End();
}

void FileSystemEditorPanel::OpenContextMenu() {
  ImGui::Selectable("Load into scene");
}
