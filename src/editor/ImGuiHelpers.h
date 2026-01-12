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
inline std::string toLower(const std::string &str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](const char c) { return std::tolower(c); });
  return result;
}
};

// next step, create a searchable popup with a "selector function" that
// preserves the original indexes
// TODO: i would prefer a reference over an pointer to the selected item
template<typename T>
inline bool SearchablePopup(const char *id, const char *title,
                            const std::vector<T> &items,
                            std::function<std::string(T)> getName,
                            std::function<bool(T)> validateItem,
                            T& outSelectedItem, bool caseInsensitive = true){
  struct PopupState {
    char searchBuffer[256] = "";
    bool firstFrame = true;
    int tempSelection = -1;
  };
  static std::map<std::string, PopupState> states;

  std::string popupId = std::string("##SearchablePopup_") + id;
  PopupState &state = states[popupId];

  bool shouldClose = false;
  bool result = false;

  if (ImGui::BeginPopupModal(popupId.c_str(), nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();
    ImGui::Separator();
    if (state.firstFrame) {
      LOG_INFO << "Set keyboard focus";
      ImGui::SetKeyboardFocusHere();
      state.firstFrame = false;
    }
    ImGui::InputText("##search", state.searchBuffer,
                     sizeof(state.searchBuffer));

    ImGui::Separator();

    ImGui::BeginChild("##ItemList", ImVec2(200, 200), true);
    for (size_t i = 0; i < items.size(); i++) {
      const T &item = items[i];
      if(!validateItem(item)) continue;
      std::string name = getName(item);

      // skip if it doesn't match search, case insensitive
      if (caseInsensitive) {
        std::string searchLower = toLower(std::string(state.searchBuffer));
        if (!searchLower.empty()) {
          std::string itemLower = toLower(name);
          if (itemLower.find(searchLower) == std::string::npos)
            continue;
        }
      }

      if (ImGui::Selectable(name.c_str(), state.tempSelection == (int)i)) {
        state.tempSelection = i;
      }
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)) {
      state.tempSelection = -1;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    if (state.tempSelection >= 0 && state.tempSelection < items.size()) {
      std::string bldStr = "Selected: " + getName(items[state.tempSelection]);
      ImGui::Text("%s", bldStr.c_str());
    } else {
      ImGui::Text("Selected: None");
    }
    ImGui::PopStyleColor();

    // add a cancel button
    if (ImGui::Button("Ok")) {
      shouldClose = true;
      if (state.tempSelection >= 0 && state.tempSelection < items.size()) {
        LOG_INFO << "Clicked ok and setting selection";
        outSelectedItem = items[state.tempSelection];
        LOG_INFO << "Set selected item to " ;
          LOG_INFO << getName(outSelectedItem);
        result = true;
      }
      else{
        LOG_ERROR << "Bounds check failed!";
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      shouldClose = true;
    }

    if (shouldClose) {
      state.searchBuffer[0] = '\0';
      state.tempSelection = -1;
      state.firstFrame = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  } else {
    state.firstFrame = true;
    state.tempSelection = -1;
  }
  return result;
}

template<typename T>
inline bool SearchablePopup(const char *id, const char *title,
                            const std::vector<T> &items,
                            std::function<std::string(T)> getName,
                            T& outSelectedItem, bool caseInsensitive = true){
  struct PopupState {
    char searchBuffer[256] = "";
    bool firstFrame = true;
    int tempSelection = -1;
  };
  static std::map<std::string, PopupState> states;

  std::string popupId = std::string("##SearchablePopup_") + id;
  PopupState &state = states[popupId];

  bool shouldClose = false;
  bool result = false;

  if (ImGui::BeginPopupModal(popupId.c_str(), nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();
    ImGui::Separator();
    if (state.firstFrame) {
      LOG_INFO << "Set keyboard focus";
      ImGui::SetKeyboardFocusHere();
      state.firstFrame = false;
    }
    ImGui::InputText("##search", state.searchBuffer,
                     sizeof(state.searchBuffer));

    ImGui::Separator();

    ImGui::BeginChild("##ItemList", ImVec2(200, 200), true);
    for (size_t i = 0; i < items.size(); i++) {
      const T &item = items[i];
      std::string name = getName(item);

      // skip if it doesn't match search, case insensitive
      if (caseInsensitive) {
        std::string searchLower = toLower(std::string(state.searchBuffer));
        if (!searchLower.empty()) {
          std::string itemLower = toLower(name);
          if (itemLower.find(searchLower) == std::string::npos)
            continue;
        }
      }

      if (ImGui::Selectable(name.c_str(), state.tempSelection == (int)i)) {
        state.tempSelection = i;
      }
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)) {
      state.tempSelection = -1;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    if (state.tempSelection >= 0 && state.tempSelection < items.size()) {
      std::string bldStr = "Selected: " + getName(items[state.tempSelection]);
      ImGui::Text("%s", bldStr.c_str());
    } else {
      ImGui::Text("Selected: None");
    }
    ImGui::PopStyleColor();

    // add a cancel button
    if (ImGui::Button("Ok")) {
      shouldClose = true;
      if (state.tempSelection >= 0 && state.tempSelection < items.size()) {
        outSelectedItem = items[state.tempSelection];
        result = true;
      }
      else{
        LOG_ERROR << "Bounds check failed!";
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      shouldClose = true;
    }

    if (shouldClose) {
      state.searchBuffer[0] = '\0';
      state.tempSelection = -1;
      state.firstFrame = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  } else {
    state.firstFrame = true;
    state.tempSelection = -1;
  }
  return result;
}
