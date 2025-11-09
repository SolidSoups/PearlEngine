#pragma once

#include <memory>
#include <vector>

class Renderable;
class Camera;

class Scene{
public:
  Scene() = default;
  ~Scene() = default;

  // Object management
  void AddObject(std::unique_ptr<Renderable> object);
  void RemoveObject(Renderable* object);
  void Clear();

  // Scene operations
  void Update();
  void Render(Camera& camera);

  // Query
  size_t GetObjectCount() const { return m_Objects.size(); }

private:
  std::vector<std::unique_ptr<Renderable>> m_Objects;
};
