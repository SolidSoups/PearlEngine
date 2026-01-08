#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

#include "imgui.h"

#include "Logger.h"
#include "imgui_internal.h"

namespace {
inline std::string toLower(const std::string& str){
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](const char c){return std::tolower(c); });
  return result;
}
};

inline bool SearchablePopup(const char *id,const char*title, const std::vector<std::string> &items, int* selectedIndex, bool caseInsensitive = true){
  struct PopupState {
    char searchBuffer[256] = "";
    bool firstFrame = true;
    int tempSelection = -1;
  };
  static std::map<std::string, PopupState> states;

  std::string popupId = std::string("##SearchablePopup_") + id;
  PopupState& state = states[popupId];

  bool shouldClose = false;
  bool result = false;


  if (ImGui::BeginPopupModal(popupId.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f,1.0f));
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();
    ImGui::Separator();
    if(state.firstFrame){
      LOG_INFO << "Set keyboard focus";
      ImGui::SetKeyboardFocusHere();
      state.firstFrame = false;
    }
    ImGui::InputText("##search", state.searchBuffer, sizeof(state.searchBuffer));

    ImGui::Separator();

    ImGui::BeginChild("##ItemList", ImVec2(200, 200), true);
    for(size_t i = 0; i< items.size(); i++){
      const std::string& item = items[i];

      // skip if it doesn't match search, case insensitive 
      if(caseInsensitive){
        std::string searchLower = toLower(std::string(state.searchBuffer));
        if(!searchLower.empty()){
          std::string itemLower = toLower(item);
          if(itemLower.find(searchLower) == std::string::npos)
            continue;
        }
      }

      if(ImGui::Selectable(item.c_str(), state.tempSelection == (int)i)){
        state.tempSelection = i;
      }
    }

    if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)){
      state.tempSelection = -1;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    if(state.tempSelection >= 0){
      std::string bldStr = "Selected: " + items[state.tempSelection];
      ImGui::Text("%s", bldStr.c_str());
    }
    else{
      ImGui::Text("Selected: None");
    }
    ImGui::PopStyleColor();

    // add a cancel button
    if(ImGui::Button("Ok")){
      if(state.tempSelection >= 0){
        *selectedIndex = state.tempSelection;
        result = true;
        shouldClose = true;
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel")){
      shouldClose = true;
    }

    if(shouldClose){
      state.searchBuffer[0] = '\0';
      state.tempSelection = -1;
      state.firstFrame = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
  else {
    state.firstFrame = true;
    state.tempSelection = -1;
  }
  return result;
}
