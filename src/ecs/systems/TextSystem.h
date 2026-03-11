#pragma once

#include <glm/glm.hpp>
#include "ecs_system.h"

struct ShaderData;
struct TextComponent;
struct TextureData;
class Material;
class ScriptEngine;
class InputManager;

class TextSystem : public ecs::System {
public:
  void initializeResources(glm::vec2* viewport, glm::vec2* viewportPos, ScriptEngine* scriptEngine);
  void render();
  void generateValidTexts();
  void generateTextMesh(TextComponent& aTextComp);
  void checkButtonClicks();

private:
  // Returns the location of the character on the atlas
  // Returns {-1, -1} on failure
  glm::ivec2 getCharacterCoord(char character);
  std::shared_ptr<ShaderData> myTextShader;
  std::shared_ptr<Material> myTextMaterial;
  std::shared_ptr<TextureData> myTextAtlas;

  glm::ivec2 charSize{8, 12}; // pixels
  glm::ivec2 atlasSize{16, 6}; // characters

  glm::vec2* myViewportSize = nullptr;
  glm::vec2* myViewportPos = nullptr;
  ScriptEngine* myScriptEngine = nullptr;
  InputManager* myInputManager = nullptr;
};
