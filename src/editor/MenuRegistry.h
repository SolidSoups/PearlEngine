#pragma once

#include <functional>
#include <stdexcept>
#include <string>

struct MenuItem{
  std::string path; // Ex. 'File/Close' or 'Tools/Debug/Clear console'
  std::function<void()> callback;
  bool* toggleptr = nullptr; // optional, for toggling on/off 
  
  MenuItem(std::string p, std::function<void()> cb, bool* toggle = nullptr)
    : path(std::move(p)), callback(std::move(cb)), toggleptr(toggle)
  {
    if(!toggleptr && !callback){
      throw std::runtime_error("MenuItem must have either a callback or toggle pointer");
    }
  }
};

class MenuRegistry{
public:
  static MenuRegistry& Get(){
    static MenuRegistry instance;
    return instance;
  }

  void Register(const std::string& path, std::function<void()> callback){
    m_MenuItems.push_back({path, callback, nullptr});
  }
  void Register(const std::string& path, bool* toggle = nullptr){
    m_MenuItems.push_back({path, [](){}, toggle});
  }

  const std::vector<MenuItem>& GetMenuItems() const{
    return m_MenuItems;
  }

private:
  std::vector<MenuItem> m_MenuItems;
};
