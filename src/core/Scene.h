#pragma once

#include <cstdio>
#include <memory>
#include <strings.h>
#include <vector>
#include "GameObject.h"

class Camera;

class Scene{
public:
  Scene() = default;
  ~Scene() = default;

  // Object management
  GameObject* CreateGameObject(std::string name = "gameObject");
  inline void Clear() { m_GameObjects.clear(); }

  // Scene operations
  void Update();
  void Render(Camera& camera);

  GameObject* GetSelectedGameObject(){
    if(selectedIndex < 0 || selectedIndex >= m_GameObjects.size()) return nullptr;

    return m_GameObjects[selectedIndex].get();
  }

  void SetSelectedIndex(size_t index){
    if(index < 0 || index >= m_GameObjects.size())
      selectedIndex = -1;
    else
      selectedIndex = index;
  }

  // Query
  inline size_t GetObjectCount() const { return m_GameObjects.size(); }
  inline const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return m_GameObjects; };
private:
  std::vector<std::unique_ptr<GameObject>> m_GameObjects;
  EntityID m_NextObjectID = 1;

  size_t selectedIndex = 0;
};
