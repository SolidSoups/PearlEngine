#include "InputInspectorPanel.h"

#include "TextureManager.h"
#include <imgui.h>

#include "ServiceLocator.h"
#include "MenuRegistry.h"

InputInspectorPanel::InputInspectorPanel()
 : EditorPanel("Input Inspector"){}

void InputInspectorPanel::OnInit(){
  MenuRegistry::Get().Register("Windows/InputManager", &m_IsOpen);
  auto& texManager = ServiceLocator::Get<TextureManager>();
  m_CheckTex = texManager.load("assets/checker.png");
  m_CrossTex = texManager.load("assets/cross.png");
}

void InputInspectorPanel::OnImGuiRender(){
  if(!m_IsOpen) return;

  ImGui::Begin(m_Name.c_str());
  if(!manager)
    manager = &ServiceLocator::Get<InputManager>();

  DrawChords();

  ImGui::End();
}



void InputInspectorPanel::DrawChords(){
  if(ImGui::CollapsingHeader("Chords")) {
    auto chords = manager->GetChords();   
    for(int i = 0; i < chords.size(); i++){
      ImGui::PushID(i);
      std::string name = "Chord " + std::to_string(i);
      if(ImGui::CollapsingHeader(name.c_str())){
        DrawChord(chords[i]);
      }
      ImGui::PopID();
    }
  }
}

void InputInspectorPanel::DrawChord(const InputManager::Chord& chord){
  if(ImGui::BeginTable("#Chordtable", 2)){
    for(int i=0; i<chord.keys.size(); i++){
      int key = chord.keys[i];
      bool active = i <= chord.progress-1;

      // get the key name
      std::string name="?";
      auto it = KeyToString.find(key);
      if(it != KeyToString.end())
        name = it->second;
  
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::TextUnformatted(name.c_str());

      ImGui::TableSetColumnIndex(1);
      if(active)
        ImGui::Image((ImTextureID)(uintptr_t)m_CheckTex->id, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0));
    } 
    ImGui::EndTable();
  }   
}
