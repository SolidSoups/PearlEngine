#include "TextSystem.h"

#include "ServiceLocator.h"
#include "ShaderManager.h"
#include "ShaderData.h"
#include "TextureManager.h"
#include "TextureData.h"
#include "Material.h"
#include "TextComponent.h"
#include "Renderer.h"
#include "TextMesh.h"
#include "TransformComponent.h"
#include "ScriptComponent.h"
#include "ScriptEngine.h"
#include "InputManager.h"
#include "Logger.h"

#include "ecs_system_impl.h"

#include <glm/gtc/matrix_transform.hpp>

#define TEXT_VERT_PATH "shaders/textVert.glsl"
#define TEXT_FRAG_PATH "shaders/textFrag.glsl"
#define TEXT_ATLAS_PATH "assets/ui/font_atlas.png"

void TextSystem::initializeResources(ScriptEngine *scriptEngine) {
  myTextShader =
      ServiceLocator::Get<ShaderManager>().load(TEXT_VERT_PATH, TEXT_FRAG_PATH);

  TextureConfig cfg;
  cfg.minFilter = GL_NEAREST;
  cfg.magFilter = GL_NEAREST;
  cfg.flipY = true;
  myTextAtlas =
      ServiceLocator::Get<TextureManager>().load(TEXT_ATLAS_PATH, cfg);
  myTextMaterial = std::make_shared<Material>(myTextShader);
  myTextMaterial->setTexture("uTextAtlas", myTextAtlas);

  myScriptEngine = scriptEngine;
  myInputManager = &ServiceLocator::Get<InputManager>();
}

void TextSystem::render() {
  for (ecs::Entity e : Entities) {
    auto &transform = Get<TransformComponent>(e);
    auto &text = Get<TextComponent>(e);
    if (!text.isVisible)
      continue;
    if (!text.mesh)
      continue;

    myTextMaterial->setVec3("uColor", text.color);

    Renderer::Submit(transform, text.mesh, myTextMaterial);
  }
}

void TextSystem::generateValidTexts() {
  for (ecs::Entity e : Entities) {
    auto &transform = Get<TransformComponent>(e);
    auto &text = Get<TextComponent>(e);

    // skip valid meshes, but regenerate if text is dirty
    if (text.mesh && !text.isDirty)
      continue;

    generateTextMesh(text);
  }
}
void TextSystem::generateTextMesh(TextComponent &aTextComp) {
  if (aTextComp.text.empty()) {
    aTextComp.mesh = nullptr;
    aTextComp.isDirty = false;
    return;
  }

  // The mesh generation is quite simple,
  // it just generates a straight line of faces
  // should be structured like so px py uvx uvy
  // generate a face for each character
  //
  //  p0 uv0 -- p1, uv1
  //    |     / |
  //    |   /   |
  //    | /     |
  //  p2 uv2 -- p3, uv3
  //
  //
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  unsigned int vertexIndex = 0;
  const float ACHAR_WIDTH = charSize.x;
  const float ACHAR_HEIGHT = charSize.y;
  const float ATLAS_X_STEP = 1.f / atlasSize.x;
  const float ATLAS_Y_STEP = 1.f / atlasSize.y;

  size_t textLength = aTextComp.text.size();
  for (size_t i = 0; i < textLength; i++) {
    char character = aTextComp.text[i];

    // get the atlas location of the character on the atlas
    glm::ivec2 atlasLocation = getCharacterCoord(character);

    float topLeftPositionX = i * ACHAR_WIDTH;
    float topLeftPositionY = 0;
    float topLeftUVX = (float)atlasLocation.x / atlasSize.x;
    float topLeftUVY = (float)atlasLocation.y / atlasSize.y;

    // top-left point
    vertices.push_back(topLeftPositionX);
    vertices.push_back(topLeftPositionY);
    vertices.push_back(topLeftUVX);
    vertices.push_back(topLeftUVY);

    // bottom-left point
    vertices.push_back(topLeftPositionX);
    vertices.push_back(topLeftPositionY + ACHAR_HEIGHT);
    vertices.push_back(topLeftUVX);
    vertices.push_back(topLeftUVY + ATLAS_Y_STEP);

    // top-right point
    vertices.push_back(topLeftPositionX + ACHAR_WIDTH);
    vertices.push_back(topLeftPositionY);
    vertices.push_back(topLeftUVX + ATLAS_X_STEP);
    vertices.push_back(topLeftUVY);

    // bottom-right point
    vertices.push_back(topLeftPositionX + ACHAR_WIDTH);
    vertices.push_back(topLeftPositionY + ACHAR_HEIGHT);
    vertices.push_back(topLeftUVX + ATLAS_X_STEP);
    vertices.push_back(topLeftUVY + ATLAS_Y_STEP);

    // Stitch together points into triangels
    //  TOP TRIANGLE
    indices.push_back(vertexIndex);
    indices.push_back(vertexIndex + 1);
    indices.push_back(vertexIndex + 2);
    // BOTTOM TRIANGLE
    indices.push_back(vertexIndex + 1);
    indices.push_back(vertexIndex + 3);
    indices.push_back(vertexIndex + 2);

    vertexIndex += 4; // increase index for next face
  }

  aTextComp.mesh = std::make_shared<TextMesh>(vertices, indices);
  aTextComp.isDirty = false;
}

void TextSystem::checkButtonClicks() {
  // move hover state from current to prev frame
  mHoverPrevFrame = std::move(mHoverThisFrame);
  mHoverThisFrame.clear();

  if (!myInputManager)
    return;

  // get mouse state
  glm::vec2 mousePos;
  if (!myInputManager->GetViewportMousePosition(mousePos)) {
    return;
  }

  // We want to only iterate on valid buttons,
  // so we cache that first so that if a button updates the
  // state of another button, we don't get a state issue
  std::vector<ecs::Entity> validTextComps;
  for (ecs::Entity e : Entities) {
    auto &text = Get<TextComponent>(e);
    if (!text.isButton || !text.isVisible || !text.mesh) {
      continue;
    }
    // Cache only valid buttons for later
    validTextComps.push_back(e);
  }

  // iterate over all valid components
  for (ecs::Entity e : validTextComps) {
    auto &text = Get<TextComponent>(e);

    // Double guard, but good to be safe
    if (!text.isButton || !text.isVisible || !text.mesh) {
      continue;
    }

    auto* scriptComp = TryGet<ScriptComponent>(e);

    auto &transform = Get<TransformComponent>(e);
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(
        model, glm::vec3(transform.position.x, transform.position.y, 0.f));
    model = glm::rotate(model, glm::radians(transform.rotation.z),
                        glm::vec3(0, 0, 1));
    model =
        glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, 1.f));

    // Transform mouse position to the TextComponent buttons local space
    glm::vec2 localMousePos =
        glm::inverse(model) * glm::vec4(mousePos, 0.f, 1.f);
    float buttonWidth = text.text.size() * charSize.x;
    float buttonHeight = charSize.y;

    // check if the mouse position is within the bounds of the button
    if (localMousePos.x >= 0 && localMousePos.x <= buttonWidth &&
        localMousePos.y >= 0 && localMousePos.y <= buttonHeight) {
      onMouseInButtonBounds(e, text, scriptComp);
    }
    else{
      // Check if we hovered this button last frame
      if(mHoverPrevFrame.count(e)){
        // Call OnHoverLeave
        if(scriptComp)
          myScriptEngine->RunScriptFunc(e, *scriptComp, "OnHoverLeave");
      }
    }
  }
}

void TextSystem::onMouseInButtonBounds(ecs::Entity aEntity, const TextComponent& aTextComp, ScriptComponent* aScriptComp) {
  bool isMouseDown = myInputManager->GetMouseKeyDown(GLFW_MOUSE_BUTTON_LEFT);
  auto* scriptComp = TryGet<ScriptComponent>(aEntity);

  // set state to hovered this frame
  mHoverThisFrame.insert(aEntity);

  // Hover enter
  if(!mHoverPrevFrame.count(aEntity)){
    if(aScriptComp)
      myScriptEngine->RunScriptFunc(aEntity, *aScriptComp, "OnHoverEnter");
  }

  // trigger on click events if the mouse is down
  if (isMouseDown && aTextComp.onClick) {
    aTextComp.onClick();
  }
  if (isMouseDown && myScriptEngine) {
    if (aScriptComp) {
      myScriptEngine->RunOnClick(aEntity, *aScriptComp);
    }
  }

  // compare state and trigger correct hover events
}

glm::ivec2 TextSystem::getCharacterCoord(char character) {
  const std::unordered_map<char, glm::ivec2> charToUV = {
      {' ', {0, 0}},   {'!', {1, 0}},  {'"', {2, 0}},  {'#', {3, 0}},
      {'%', {5, 0}},   {'\'', {7, 0}}, {'(', {8, 0}},  {')', {9, 0}},
      {'*', {10, 0}},  {'+', {11, 0}}, {',', {12, 0}}, {'-', {13, 0}},
      {'.', {14, 0}},  {'/', {15, 0}},

      {'0', {0, 1}},   {'1', {1, 1}},  {'2', {2, 1}},  {'3', {3, 1}},
      {'4', {4, 1}},   {'5', {5, 1}},  {'6', {6, 1}},  {'7', {7, 1}},
      {'8', {8, 1}},   {'9', {9, 1}},  {':', {10, 1}}, {';', {11, 1}},
      {'<', {12, 1}},  {'=', {13, 1}}, {'>', {14, 1}}, {'?', {15, 1}},

      {'@', {0, 2}},   {'A', {1, 2}},  {'B', {2, 2}},  {'C', {3, 2}},
      {'D', {4, 2}},   {'E', {5, 2}},  {'F', {6, 2}},  {'G', {7, 2}},
      {'H', {8, 2}},   {'I', {9, 2}},  {'J', {10, 2}}, {'K', {11, 2}},
      {'L', {12, 2}},  {'M', {13, 2}}, {'N', {14, 2}}, {'O', {15, 2}},

      {'P', {0, 3}},   {'Q', {1, 3}},  {'R', {2, 3}},  {'S', {3, 3}},
      {'T', {4, 3}},   {'U', {5, 3}},  {'V', {6, 3}},  {'W', {7, 3}},
      {'X', {8, 3}},   {'Y', {9, 3}},  {'Z', {10, 3}}, {'[', {11, 3}},
      {'\\', {12, 3}}, {']', {13, 3}}, {'^', {14, 3}}, {'_', {15, 3}},

      {'`', {0, 4}},   {'a', {1, 4}},  {'b', {2, 4}},  {'c', {3, 4}},
      {'d', {4, 4}},   {'e', {5, 4}},  {'f', {6, 4}},  {'g', {7, 4}},
      {'h', {8, 4}},   {'i', {9, 4}},  {'j', {10, 4}}, {'k', {11, 4}},
      {'l', {12, 4}},  {'m', {13, 4}}, {'n', {14, 4}}, {'o', {15, 4}},

      {'p', {0, 5}},   {'q', {1, 5}},  {'r', {2, 5}},  {'s', {3, 5}},
      {'t', {4, 5}},   {'u', {5, 5}},  {'v', {6, 5}},  {'w', {7, 5}},
      {'x', {8, 5}},   {'y', {9, 5}},  {'z', {10, 5}}, {'{', {11, 5}},
      {'|', {12, 5}},  {'}', {13, 5}}, {'~', {14, 5}}};

  // find character uv
  auto it = charToUV.find(character);
  if (it != charToUV.end()) {
    return it->second;
  }

  return {-1, -1};
}
