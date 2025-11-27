#pragma once

#include "EditorPanel.h"
#include "MaterialData.h"
#include "MenuRegistry.h"
#include "Mesh.h"
#include "Scene.h"
#include "ShaderData.h"
#include <string>
#include <filesystem>
#include "File.h"

class AssetEditorPanel : public EditorPanel {
  public:
    AssetEditorPanel(Scene &scene)
        : EditorPanel("Asset Explorer"), m_Scene(scene){
        MenuRegistry::Get().Register("Windows/Asset Explorer", &m_IsOpen);
    }

    void OnImGuiRender() override;

  private:
    void LoadAsset(const pe::FileDescriptor& file);

  private:
    Scene &m_Scene;
    const std::string c_AssetDirectory = "project_assets/";
};
