#pragma once
#include "imgui.h"
#include <functional>
#include <vector>
#include <string>

template<typename T, typename GetNameFunc>
bool SearchablePopup(const char* id, const std::vector<T>& items,
                   GetNameFunc getName,
                     T* outSelected){
  std::string popupId = std::string("##SearchablePopup_") + id;

  if(ImGui::BeginPopup(popupId.c_str())){
    ImGui::Text("Popup is open");
    ImGui::Separator();

    ImGui::Text("Search will go here");

    ImGui::Text("Item list will go here");
    ImGui::Text("Item list will go here");
    ImGui::Text("Item list will go here");
    ImGui::Text("Item list will go here");


    ImGui::EndPopup();
  }
  return false;
}

