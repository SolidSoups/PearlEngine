#pragma once

#include "EditorPanel.h"
#include "MaterialData.h"
#include "MenuRegistry.h"
#include "Scene.h"
#include "ShaderData.h"
#include <string>

class ProjectEditorPanel : public EditorPanel {
  public:
    ProjectEditorPanel(Scene &scene, MaterialHandle matHandle)
        : EditorPanel("Project Explorer"), m_Scene(scene), m_MatHandle(matHandle) {
        MenuRegistry::Get().Register("Windows/Project Explorer", &m_IsOpen);
    }

    void OnImGuiRender() override;

  private:
    void CreateSceneObjectFromFile(const std::string filepath);

  private:
    Scene &m_Scene;
    MaterialHandle m_MatHandle;
    const std::string c_AssetDirectory = "assets/";
    std::vector<std::string> m_Files;
};
