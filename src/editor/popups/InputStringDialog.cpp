#include "InputStringDialog.h"

#include <imgui.h>

InputStringDialog::InputStringDialog(const std::string& title, std::function<void(const std::string&)> callback)
  : mTitle(title), mCallback(callback)
{}

void InputStringDialog::Draw() {
  // track state over several calls (TODO: could be members honestly)
  struct PopupState {
    char textBuffer[256] = "";
    bool firstFrame = true;
  };
  static PopupState state;

  // open popup if not open yet
  if(!ImGui::IsPopupOpen("##InputStringDialog")) {
    memset(state.textBuffer, 0, sizeof(state.textBuffer));
    state.firstFrame = true;
    ImGui::OpenPopup("##InputStringDialog");
  } 

  // track popup state
  bool shouldClose = false;
  bool result = false;
  
  if(ImGui::BeginPopupModal("##InputStringDialog", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
    // nice colored header
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
    ImGui::Text("%s##popupTitle", mTitle.c_str());
    ImGui::PopStyleColor();

    // draw input box
    // set focus on first frame (doing it every frame will be bad)
    if(state.firstFrame){
      ImGui::SetKeyboardFocusHere();
      state.firstFrame = false;
    }
    ImGui::InputText("##search", state.textBuffer, sizeof(state.textBuffer));

    // confirm button
    if(ImGui::Button("OK")){
      shouldClose = true;
      result = true;
    }

    ImGui::SameLine();

    // cancel button
    if(ImGui::Button("CANCEL")){
      shouldClose = true;
    }
    
    // Enter for confirm
    if(ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)){
      shouldClose = true;
      result = true;
    }
    // Escape for cancel
    if(ImGui::IsKeyPressed(ImGuiKey_Escape)){
      shouldClose = true;
    }

    // time to close popup?
    if(shouldClose){
      mInput = std::string(state.textBuffer); 
      ImGui::CloseCurrentPopup();

      // if we have a result, give it to the callback
      if(result){
        mCallback(mInput);
      }
      // destroy me
      DestroySelf();
    }

    ImGui::EndPopup();
  }
}


