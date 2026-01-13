#include "MeshPopupDialog.h"

#include "ImGuiHelpers.h"
#include "ServiceLocator.h"
#include "MeshManager.h"
#include "imgui.h"

MeshPopupDialog::MeshPopupDialog(
    std::function<void(std::shared_ptr<Mesh>)> callback)
    : m_Callback(callback) {
  m_FoundFiles = FileSystem::queryFiles(".obj");
}

void MeshPopupDialog::Draw() {
  if (!ImGui::IsPopupOpen("##SearchablePopup_MeshPopupDialog")) {
    ImGui::OpenPopup("##SearchablePopup_MeshPopupDialog");
  }

  FileSystem::FileDescriptor selectedFile;
  if (SearchablePopup<FileSystem::FileDescriptor>(
          "MeshPopupDialog", "Select a mesh", m_FoundFiles,
          [](FileSystem::FileDescriptor desc) { return desc.stem; },
          selectedFile)) {
    std::shared_ptr<Mesh> loadedMesh =
        ServiceLocator::Get<MeshManager>().loadOBJ(
            selectedFile.localPath.c_str());
    if (loadedMesh && m_Callback)
      m_Callback(loadedMesh);
    DestroySelf();
  }

  if (!ImGui::IsPopupOpen("##SearchablePopup_MeshPopupDialog")) {
    DestroySelf();
  }
}
