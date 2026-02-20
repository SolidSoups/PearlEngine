#pragma once


#include <memory>
#include "EditorPanel.h"

#include "InputManager.h"

struct TextureData;

struct TextureData;

class InputInspectorPanel : public EditorPanel {
public:
  InputInspectorPanel();

  void OnInit() override;
  void OnImGuiRender() override;

private:
  void DrawChords();
  void DrawChord(const InputManager::Chord& chord);

private:
  InputManager* manager;
  std::shared_ptr<TextureData> m_CheckTex;
  std::shared_ptr<TextureData> m_CrossTex;
};
