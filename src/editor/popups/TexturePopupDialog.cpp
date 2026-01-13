#include "TexturePopupDialog.h"

#include "ImGuiHelpers.h"
#include "ServiceLocator.h"
#include "TextureManager.h"
#include "imgui.h"

TexturePopupDialog::TexturePopupDialog(
    std::function<void(std::shared_ptr<TextureData>)> callback)
    : m_Callback(callback) {
  const std::vector<std::string> allowedExtensions = {".png", ".jpg"};
  m_FoundFiles = FileSystem::queryFiles(allowedExtensions);
}

void TexturePopupDialog::Draw() {
  if (!ImGui::IsPopupOpen("##SearchablePopup_TexturePopupDialog")) {
    ImGui::OpenPopup("##SearchablePopup_TexturePopupDialog");
  }

  FileSystem::FileDescriptor selectedFile;
  if (SearchablePopup<FileSystem::FileDescriptor>(
          "TexturePopupDialog", "Select a texture", m_FoundFiles,
          [](FileSystem::FileDescriptor desc) { return desc.stem; },
          selectedFile)) {
    std::shared_ptr<TextureData> loadedMesh =
        ServiceLocator::Get<TextureManager>().load(
            selectedFile.localPath.c_str());
    if (loadedMesh && m_Callback)
      m_Callback(loadedMesh);
    DestroySelf();
  }

  if (!ImGui::IsPopupOpen("##SearchablePopup_TexturePopupDialog")) {
    DestroySelf();
  }
}
