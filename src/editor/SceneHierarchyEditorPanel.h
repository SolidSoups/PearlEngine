#pragma once

#include "EditorPanel.h"
#include "Scene.h"
#include "MaterialData.h"

class SceneHierarchyEditorPanel : public EditorPanel{
public:
  SceneHierarchyEditorPanel(Scene& scene, MaterialHandle pearlHandle, 
                            MaterialHandle sunnyHandle) 
  : EditorPanel("Scene Hierarchy")
    , m_Scene(scene)
    , m_PearlHandle(pearlHandle) 
    , m_SunnyHandle(sunnyHandle)
  {}

public:
  void OnImGuiRender() override;

private:
  Scene& m_Scene;
  MaterialHandle m_PearlHandle;
  MaterialHandle m_SunnyHandle;
};
