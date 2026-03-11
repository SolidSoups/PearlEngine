#include "TextComponentEditor.h"


void TextComponentEditor::OnDrawComponent(void* target, [[maybe_unused]] ecs::Entity entity){
  constexpr float labelWidth = 120.f; 
  TextComponent* textComp = static_cast<TextComponent*>(target);
  if(!textComp) return;
  
  // font atlas file
  static char textBuf[256] = ""; 
  if(entity != myLastEntity){
    myLastEntity = entity;
    strncpy(textBuf, textComp->text.c_str(), sizeof(textBuf)-1);
    textBuf[sizeof(textBuf)-1] = '\0';
    // TODO: sync texture creator
  }

  // render color
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Color");
  ImGui::SameLine(labelWidth);
  ImGui::ColorEdit3("##Color", &textComp->color.x);

  // render size
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Size");
  ImGui::SameLine(labelWidth);
  if(ImGui::DragInt("##Size", &textComp->size, 1)){
    textComp->isDirty = true;
  }
  
  // render text box
  ImGui::Text("Text");
  if(ImGui::InputTextMultiline("##bigtext", textBuf, sizeof(textBuf), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10))){
    textComp->text = textBuf;
    textComp->isDirty = true;
  }

}

