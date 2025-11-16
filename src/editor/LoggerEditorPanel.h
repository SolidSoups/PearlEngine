#pragma once

#include "EditorPanel.h"
#include "MenuRegistry.h"
#include "Logger.h"

class LoggerEditorPanel : public EditorPanel{
public:
  LoggerEditorPanel() 
  : EditorPanel("Logging Console")
  , m_Logger(Logger::Get()) 
  {
    MenuRegistry::Get().Register("Windows/Logger", &m_IsOpen); 
  }


public:
  void OnImGuiRender() override;

private:
  Logger& m_Logger;
  void DrawMenuBar();
  void DrawLogArea();
};
