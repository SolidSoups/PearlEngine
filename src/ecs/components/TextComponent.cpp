#include "TextComponent.h"

#include "ServiceLocator.h"
#include "TextureManager.h"

void to_json(json& j, const TextComponent&t){
  j["text"] = t.text;
  j["color"] = t.color;
  j["isButton"] = t.isButton;
}
void from_json(const json& j, TextComponent& t){
	t.text =   j["text"];
  t.color = j["color"];
  t.isButton = j.value("isButton", false);
}
