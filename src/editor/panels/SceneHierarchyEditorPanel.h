#pragma once

#include "EditorPanel.h"
#include "Scene.h"
#include "MaterialData.h"

class SceneHierarchyEditorPanel : public EditorPanel{
public:
  SceneHierarchyEditorPanel(Scene& scene, MaterialHandle pearlHandle, 
                            MaterialHandle sunnyHandle) ;

public:
  void OnImGuiRender() override;

private:
  void DrawSceneHierarchy();  
  void DrawContextMenu();
private:
  Scene& m_Scene;
  MaterialHandle m_PearlHandle;
  MaterialHandle m_SunnyHandle;

};
