#include "MenuBar.h"

#include <map>
#include <string>
#include <vector>

#include "imgui.h"

#include "MenuRegistry.h"

void MenuBar::RenderMenuBar(){
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

    ImGui::EndMainMenuBar();
  }
}
