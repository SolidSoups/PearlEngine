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
  void initializeResources(ScriptEngine* scriptEngine);
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

  ScriptEngine* myScriptEngine = nullptr;
  InputManager* myInputManager = nullptr;
};
