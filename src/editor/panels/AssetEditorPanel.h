#pragma once

#include "AssetDescriptor.h"
#include "EditorPanel.h"
#include "MaterialData.h"
#include "MenuRegistry.h"
#include "ServiceLocator.h"
#include "imgui.h"
#include <filesystem>
#include <string>
#include <unordered_map>
#include "Scene.h"

class AssetEditorPanel : public EditorPanel {
public:
  AssetEditorPanel(const MaterialHandle &handle, ServiceLocator *locator)
      : EditorPanel("Asset Explorer"), m_matHandle(handle), r_Scene(locator->Get<Scene>()) {
    MenuRegistry::Get().Register("Windows/Asset Explorer", &m_IsOpen);
    LoadColors();
  }

  void OnImGuiRender() override;

private:
  void LoadAsset(const pe::AssetDescriptor &file);

private:
  MaterialHandle m_matHandle;
  std::unordered_map<std::string, ImVec4> ExtensionToColor;
  const std::filesystem::path c_EditorSettingsPath{"editor_settings"};
  void LoadColors();

private:
  Scene& r_Scene;
};
