#pragma once

#include <memory>
#include <vector>
#include "GameObject.h"

class Camera;

class Scene{
public:
  Scene() = default;
  ~Scene() = default;

  // Object management
  GameObject* CreateGameObject();
  inline void Clear() { m_GameObjects.clear(); }

  // Scene operations
  void Update();
  void Render(Camera& camera);

  // Query
  size_t GetObjectCount() const { return m_GameObjects.size(); }

private:
  std::vector<std::unique_ptr<GameObject>> m_GameObjects;
  EntityID m_NextObjectID = 1;
};
