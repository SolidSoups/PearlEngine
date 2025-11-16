#pragma once

#include "EditorPanel.h"
#include "MenuRegistry.h"

class LoggerEditorPanel : public EditorPanel{
public:
  LoggerEditorPanel() : EditorPanel("Logging Console"){
    MenuRegistry::Get().Register("Windows/Logger", &m_IsOpen); 
  }


public:
  void OnImGuiRender() override;
};
