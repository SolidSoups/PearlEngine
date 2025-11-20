#pragma once

#include "EditorPanel.h"
#include "MenuRegistry.h"
#include "Scene.h"
#include "ShaderData.h"
#include "MaterialData.h"
#include <string>

class AssetEditorPanel : public EditorPanel{
public:
  AssetEditorPanel(Scene& scene, MaterialHandle matHandle)
    : EditorPanel("Assets")
    , m_Scene(scene) 
    , m_MatHandle(matHandle)
  {

    MenuRegistry::Get().Register("Windows/Assets", &m_IsOpen);
  }

  void OnImGuiRender() override;

private:
  void RefreshAssetPaths();
  void CreateSceneObjectFromFile(const std::string filepath);

private:
  Scene& m_Scene;
  MaterialHandle m_MatHandle;
  const std::string c_AssetDirectory = "assets/";
  std::vector<std::string> m_Files;
};
