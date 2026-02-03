#include "UserGUI.h"

#include <cstring>

#include <imgui.h>

#include "IPopupDialog.h"
#include "Logger.h"
#include "MeshPopupDialog.h"
#include "TexturePopupDialog.h"
#include "FilePopupDialog.h"

IPopupDialog *UserGUI::m_CurrentDialog = nullptr;

void UserGUI::StartMeshPopup(
    std::function<void(std::shared_ptr<Mesh>)> callback) {
  if (!m_CurrentDialog)
    m_CurrentDialog = new MeshPopupDialog(callback);
  else
    LOG_WARNING << "Cannot start more than one dialog at a time";
}

void UserGUI::StartTexturePopup(
    std::function<void(std::shared_ptr<TextureData>)> callback) {
  if (!m_CurrentDialog)
    m_CurrentDialog = new TexturePopupDialog(callback);
  else
    LOG_WARNING << "Cannot start more than one dialog at a time";
}

void UserGUI::Render() {
  if (!m_CurrentDialog)
    return;

  if (!m_CurrentDialog->NeedsDestroy()) {
    m_CurrentDialog->Draw();
  } else {
    delete m_CurrentDialog;
    m_CurrentDialog = nullptr;
  }
}

void UserGUI::StartFilePopup(std::function<void(const std::string &)> callback,
                             const std::vector<std::string> &extensions) {
  if (!m_CurrentDialog)
    m_CurrentDialog = new FilePopupDialog(callback, extensions);
  else
    LOG_WARNING << "Cannot start more than one dialog at a time";
}

bool UserGUI::DrawFile(std::string &filePath) {
  ImGui::BeginDisabled();
  static char displayBuf[256];
  strncpy(displayBuf, filePath.c_str(), sizeof(displayBuf) - 1);
  displayBuf[sizeof(displayBuf) - 1] = '\0';
  ImGui::SetNextItemWidth(-10);
  ImGui::InputText("##filepath", displayBuf, sizeof(displayBuf),
                   ImGuiInputTextFlags_ReadOnly);
  ImGui::EndDisabled();

  static bool fileChanged = false;
  if (ImGui::Button("Browse..", ImVec2(-10, 0))) {
    const std::vector<std::string> extensions = {".png", ".jpg", ".tga"};
    StartFilePopup(
        [&filePath](const std::string &newFilePath) { filePath = newFilePath; fileChanged = true; },
        extensions);
  }
  if(fileChanged){
    fileChanged = false;
    return true;
  }
  return false;
}

void UserGUI::Destroy() {
  if (m_CurrentDialog)
    delete m_CurrentDialog;
}
