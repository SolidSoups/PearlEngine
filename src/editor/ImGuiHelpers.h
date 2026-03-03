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
    char lastSearchBuffer[256] = "";
    bool firstFrame = true;
    int tempSelection = -1;
    bool scrollToSelection = false;
  };
  static std::map<std::string, PopupState> states;

  // track id and state
  std::string popupId = std::string("##SearchablePopup_") + id;
  PopupState &state = states[popupId];

  // track if we have closed or returned
  bool shouldClose = false;
  bool result = false;

  // start the popup modal
  if (ImGui::BeginPopupModal(popupId.c_str(), nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    // draw title
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();

    ImGui::Separator();

    // draw input box
    bool wasFirstFrame = state.firstFrame;
    if (state.firstFrame) {
      ImGui::SetKeyboardFocusHere();
      state.firstFrame = false;
    }
    ImGui::InputText("##search", state.searchBuffer,
                     sizeof(state.searchBuffer));

    ImGui::Separator();

    // build filtered index list
    std::string searchLower = toLower(std::string(state.searchBuffer));
    std::vector<int> visibleIndices;
    for (int i = 0; i < (int)items.size(); i++) {
      if (!validateItem(items[i])) continue;
      if (caseInsensitive && !searchLower.empty()) {
        if (toLower(getName(items[i])).find(searchLower) == std::string::npos)
          continue;
      }
      visibleIndices.push_back(i);
    }

    // auto-select first item on open
    if (wasFirstFrame && !visibleIndices.empty()) {
      state.tempSelection = visibleIndices[0];
      state.scrollToSelection = true;
      memcpy(state.lastSearchBuffer, state.searchBuffer, sizeof(state.searchBuffer));
    }

    // reset selection when search changes
    if (strcmp(state.searchBuffer, state.lastSearchBuffer) != 0) {
      state.tempSelection = visibleIndices.empty() ? -1 : visibleIndices[0];
      state.scrollToSelection = true;
      memcpy(state.lastSearchBuffer, state.searchBuffer, sizeof(state.searchBuffer));
    }

    // arrow key navigation
    int visPos = -1;
    for (int i = 0; i < (int)visibleIndices.size(); i++) {
      if (visibleIndices[i] == state.tempSelection) { visPos = i; break; }
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) && !visibleIndices.empty()) {
      visPos = (visPos + 1) % (int)visibleIndices.size();
      state.tempSelection = visibleIndices[visPos];
      state.scrollToSelection = true;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && !visibleIndices.empty()) {
      visPos = (visPos <= 0) ? (int)visibleIndices.size() - 1 : visPos - 1;
      state.tempSelection = visibleIndices[visPos];
      state.scrollToSelection = true;
    }

    // draw items...
    ImGui::BeginChild("##ItemList", ImVec2(400, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (int idx : visibleIndices) {
      std::string name = getName(items[idx]) + "##" + std::to_string(idx);

      bool selected = (state.tempSelection == idx);
      if (ImGui::Selectable(name.c_str(), selected)) {
        state.tempSelection = idx;
      }
      if (selected && state.scrollToSelection) {
        ImGui::SetScrollHereY(0.5f);
        state.scrollToSelection = false;
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

    // confirm with Enter key
    if (ImGui::IsKeyPressed(ImGuiKey_Enter) && state.tempSelection >= 0 && state.tempSelection < (int)items.size()) {
      shouldClose = true;
      outSelectedItem = items[state.tempSelection];
      result = true;
    }

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
      state.lastSearchBuffer[0] = '\0';
      state.tempSelection = -1;
      state.scrollToSelection = false;
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
  return SearchablePopup<T>(id, title, items, getName,[](T){return true;}, outSelectedItem, caseInsensitive);
}
