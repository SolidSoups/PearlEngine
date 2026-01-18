#include "FilePopupDialog.h"
#include "imgui.h"
#include "ImGuiHelpers.h"

FilePopupDialog::FilePopupDialog(
    std::function<void(const std::string &)> callback,
    const std::vector<std::string> &extensions)
    : m_Callback(callback) {
  m_FoundFiles = FileSystem::queryFiles(extensions);
}

void FilePopupDialog::Draw() {
  if (!ImGui::IsPopupOpen("##SearchablePopup_FilePopupDialog")) {
    ImGui::OpenPopup("##SearchablePopup_FilePopupDialog");
  }

  FileSystem::FileDescriptor selectedFile;
  if (SearchablePopup<FileSystem::FileDescriptor>(
          "FilePopupDialog", "Select a file", m_FoundFiles,
          [](FileSystem::FileDescriptor desc) { return desc.stem; },
          selectedFile)) {
    std::string selectedFilePath = selectedFile.localPath;
    if (m_Callback)
      m_Callback(selectedFilePath);
    DestroySelf();
  }

  if (!ImGui::IsPopupOpen("##SearchablePopup_FilePopupDialog")) {
    DestroySelf();
  }
}
