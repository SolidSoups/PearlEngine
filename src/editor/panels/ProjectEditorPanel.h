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

class ProjectEditorPanel : public EditorPanel {
  public:
    ProjectEditorPanel(Scene &scene)
        : EditorPanel("Project Explorer"), m_Scene(scene) {
        MenuRegistry::Get().Register("Windows/Project Explorer", &m_IsOpen);
    }

    void OnImGuiRender() override;

  private:
    void CreateSceneObjectWithMesh(MeshHandle meshHandle, const std::string& name);
    void LoadAsset(const pe::FileDescriptor& file);

  private:
    Scene &m_Scene;
    const std::string c_AssetDirectory = "assets/";
    std::vector<std::string> m_Files;
};
