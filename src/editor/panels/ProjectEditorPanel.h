#pragma once

#include "EditorPanel.h"
#include "MaterialData.h"
#include "MenuRegistry.h"
#include "Mesh.h"
#include "Scene.h"
#include "ShaderData.h"
#include "ServiceLocator.h"
#include "ServiceHandle.h"
#include "AssetSystem.h"
#include <string>
#include <filesystem>
#include "FileDescriptor.h"

class ProjectEditorPanel : public EditorPanel {
  public:
    ProjectEditorPanel(ServiceLocator* locator)
        : EditorPanel("Project Explorer"), r_AssetSystem(locator) {
        MenuRegistry::Get().Register("Windows/Project Explorer", &m_IsOpen);
    }

    void OnImGuiRender() override;

  private:
    void CreateSceneObjectWithMesh(MeshHandle meshHandle, const std::string& name);
    void LoadAsset(const pe::FileDescriptor& file);

  private:
    const std::string c_AssetDirectory = "assets/";
    std::vector<std::string> m_Files;
    ServiceHandle<pe::AssetSystem> r_AssetSystem;
};
