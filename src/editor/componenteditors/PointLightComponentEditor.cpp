#include "PointLightComponentEditor.h"

#include "PointLightComponent.h"
#include "imgui.h"

void PointLightComponentEditor::OnDrawComponent(IComponent* component){
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
  ImGui::DragFloat("##Intensity", &pointLightComp->data.intensity);

  ImGui::SeparatorText("Attenuation");

  // draw radius
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Radius");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##Radius", &pointLightComp->data.radius);

  // draw constant attenuation
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Constant Attenuation");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##ConstAtten", &pointLightComp->data.constantAttenuation);

  // draw constant attenuation
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Linear Attenuation");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##LinearAtten", &pointLightComp->data.linearAttenuation);

  // draw quadratic attenuation
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Quadratic Attenuation");
  ImGui::SameLine(labelWidth);
  ImGui::DragFloat("##QuadAtten", &pointLightComp->data.quadraticAttenuation);
}
