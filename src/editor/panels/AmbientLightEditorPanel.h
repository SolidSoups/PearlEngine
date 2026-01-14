#pragma once

#include "EditorPanel.h"
#include "MenuRegistry.h"
#include "Scene.h"
#include "ServiceLocator.h"

class AmbientLightEditorPanel : public EditorPanel{
public:
  AmbientLightEditorPanel()
    : EditorPanel("Ambient Light"), s_Scene(ServiceLocator::Get<Scene>()){
    MenuRegistry::Get().Register("Windows/Ambient Light", &m_IsOpen);
  }

  void OnImGuiRender() override;
private:
  Scene& s_Scene;
};
