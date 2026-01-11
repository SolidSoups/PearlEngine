#pragma once

#include "AssetDescriptor.h"
#include "EditorPanel.h"
#include "MaterialData.h"
#include "MenuRegistry.h"
#include "ServiceLocator.h"
#include "ServiceHandle.h"
#include "AssetSystem.h"
#include "ResourceSystem.h"
#include "imgui.h"
#include <filesystem>
#include <string>
#include <unordered_map>
#include "Scene.h"

class AssetEditorPanel : public EditorPanel {
public:
  AssetEditorPanel(const MaterialDataHandle &handle)
      : EditorPanel("Asset Explorer"), m_matHandle(handle), r_Scene(ServiceLocator::Get<Scene>()),
        r_AssetSystem(ServiceLocator::Get<pe::AssetSystem>()), r_ResourceSystem(ServiceLocator::Get<ResourceSystem>()) {
    MenuRegistry::Get().Register("Windows/Asset Explorer", &m_IsOpen);
    LoadColors();
  }

  void OnImGuiRender() override;

private:
  void LoadAsset(const pe::AssetDescriptor &file);

private:
  MaterialDataHandle m_matHandle;
  std::unordered_map<std::string, ImVec4> ExtensionToColor;
  const std::filesystem::path c_EditorSettingsPath{"editor_settings"};
  void LoadColors();

private:
  Scene& r_Scene;
  pe::AssetSystem& r_AssetSystem;
  ResourceSystem &r_ResourceSystem;
};
