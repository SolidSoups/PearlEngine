#include "MenuBar.h"

#include <map>
#include <string>
#include <vector>

#include "imgui.h"

#include "MenuRegistry.h"
#include "Logger.h"

void MenuBar::SetButtonCallbacks(
  std::function<void()>& onPlayClicked,
  std::function<void()>& onPauseClicked,
  std::function<void()>& onStopClicked,
  std::function<void()>& onReloadClicked,
  std::shared_ptr<TextureData>& playTex,
  std::shared_ptr<TextureData>& pauseTex,
  std::shared_ptr<TextureData>& stopTex,
  std::shared_ptr<TextureData>& reloadTex
){
  mPlayTex = std::move(playTex);
  mPauseTex = std::move(pauseTex);
  mStopTex = std::move(stopTex);
  mReloadTex = std::move(reloadTex);
  mOnPlayClicked = std::move(onPlayClicked);
  mOnPauseClicked = std::move(onPauseClicked);
  mOnStopClicked = std::move(onStopClicked);
  mOnReloadClicked = std::move(onReloadClicked);
}

void MenuBar::RenderMenuBar(uint8_t runtimeState){
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.0f));
  if(ImGui::BeginMainMenuBar()){
    // Build menu structure from registered classes
    std::map<std::string, std::vector<MenuItem>> menuStructure;

    for(const auto& item : MenuRegistry::Get().GetMenuItems()){
      // Parse "File/New" into menu="File", itemName="New"
      size_t slashPos = item.path.find_last_of('/');
      std::string menu = item.path.substr(0, slashPos);
      menuStructure[menu].push_back(item);
    }

    // Render menus
    for(const auto& [menuName, items] : menuStructure){
      if(ImGui::BeginMenu(menuName.c_str())){
        for(const auto& item : items){
          size_t slashPos = item.path.find_last_of('/');
          std::string itemName = item.path.substr(slashPos + 1);

          if(item.toggleptr){
            ImGui::MenuItem(itemName.c_str(), nullptr, item.toggleptr);
          }
          else{
            // regular menu item
            if(ImGui::MenuItem(itemName.c_str())){
              item.callback();
            }
          }
        }
        ImGui::EndMenu();
      }
    }

    // render play buttons
    float buttonSize = 20.f;
    float windowWidth = ImGui::GetWindowWidth();
    float windowHeight = ImGui::GetWindowHeight();
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    bool isRuntime = runtimeState == 1;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
    if(isRuntime){
      float totalWidth = buttonSize * 3 + spacing * 2;
      ImGui::SameLine((windowWidth - totalWidth) * 0.5f);
      ImGui::SetCursorPosY((windowHeight - buttonSize) * 0.5f);
      
      if(ImGui::ImageButton("##stop", (ImTextureID)(intptr_t)mStopTex->id, ImVec2(buttonSize, buttonSize)))
        if(mOnStopClicked) mOnStopClicked();
      ImGui::SameLine();
      ImGui::SetCursorPosY((windowHeight - buttonSize) * 0.5f);
      if(ImGui::ImageButton("##pause", (ImTextureID)(intptr_t)mPauseTex->id, ImVec2(buttonSize, buttonSize)))
        if(mOnPauseClicked) mOnPauseClicked();
      ImGui::SameLine();
      ImGui::SetCursorPosY((windowHeight - buttonSize) * 0.5f);
      if(ImGui::ImageButton("##reload", (ImTextureID)(intptr_t)mReloadTex->id, ImVec2(buttonSize, buttonSize)))
        if(mOnReloadClicked) mOnReloadClicked();
    }
    else{
      ImGui::SameLine((windowWidth - buttonSize) * 0.5f);
      ImGui::SetCursorPosY((windowHeight - buttonSize) * 0.5f);
      if(ImGui::ImageButton("##play", (ImTextureID)(intptr_t)mPlayTex->id, ImVec2(buttonSize, buttonSize)))
        if(mOnPlayClicked) mOnPlayClicked();
    }

   

    ImGui::PopStyleVar();
    

    ImGui::EndMainMenuBar();
  }
  ImGui::PopStyleVar();
}
