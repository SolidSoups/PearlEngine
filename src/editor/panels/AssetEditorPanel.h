#pragma once

#include "AssetDescriptor.h"
#include "EditorPanel.h"
#include "MaterialData.h"
#include "MenuRegistry.h"
#include "imgui.h"
#include <filesystem>
#include <string>
#include <unordered_map>

class AssetEditorPanel : public EditorPanel {
  public:
    AssetEditorPanel(const MaterialHandle &handle)
        : EditorPanel("Asset Explorer"), m_matHandle(handle) {
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
};
