#pragma once

#include "EditorPanel.h"
#include "MenuRegistry.h"
class MemoryEditorPanel : public EditorPanel{
public:
  MemoryEditorPanel() : EditorPanel("Memory") {
    MenuRegistry::Get().Register("Memory Panel", &m_IsOpen);
  }

  void OnImGuiRender() override;
};
