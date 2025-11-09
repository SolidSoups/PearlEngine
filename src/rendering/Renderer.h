#pragma once

class Camera;
class Renderable;
class Transform;

class Renderer {
public:
  // Scene management
  static void BeginScene(Camera& camera);
  static void EndScene();

  static void Submit(Renderable& renderable);

private:
  static Camera* s_ActiveCamera;
};
