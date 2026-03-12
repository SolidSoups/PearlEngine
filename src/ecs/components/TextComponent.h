#pragma once

#include <string>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include "json_common.h"

class TextMesh;

struct TextComponent {
  std::shared_ptr<TextMesh> mesh; // runtime only
  std::function<void()> onClick;  // runtime only
  std::string text;
  glm::vec3 color{1.0};
  bool isDirty = true;
  bool isVisible = true;
  bool isButton = false;

public: // serialization
  friend void to_json(json& j, const TextComponent&t);
  friend void from_json(const json& j, TextComponent& t);
};
