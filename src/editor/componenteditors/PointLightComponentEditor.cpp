#include "PointLightComponentEditor.h"

#include "PointLightComponent.h"
#include "imgui.h"

void PointLightComponentEditor::OnDrawComponent(void* component, [[maybe_unused]] ecs::Entity entity){
  PointLightComponent* pointLightComp = static_cast<PointLightComponent*>(component);
  if(!pointLightComp) return;

  constexpr float labelWidth = 180.f;

  ImGui::SeparatorText("Standard");

  // draw color
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Color");
  ImGui::SameLine(labelWidth);
  ImGui::ColorEdit4("##Color", &pointLightComp->data.color.x);

  // draw intensity
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Intensity");
  ImGui::SameLine(labelWidth);
  ImGui::SliderFloat("##Intensity", &pointLightComp->data.intensity, 0.0f, 10.0f);

  ImGui::SeparatorText("Attenuation");

  // draw radius
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Radius");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##Radius", &pointLightComp->data.radius);

  // draw constant attenuation
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Linear Attenuation");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##LinearAtten", &pointLightComp->data.linearAttenuation, 0.001f, 0.0f, 1.0f);

  // draw quadratic attenuation
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Quadratic Attenuation");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##QuadAtten", &pointLightComp->data.quadraticAttenuation, 0.001f, 0.0f, 2.0f);
}
