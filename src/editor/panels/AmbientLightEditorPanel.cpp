#include "AmbientLightEditorPanel.h"

#include "imgui.h"

void AmbientLightEditorPanel::OnImGuiRender(){
  if(!m_IsOpen) return;
  constexpr float labelWidth = 140.f;

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Ambient Color");
  ImGui::SameLine(labelWidth);
  ImGui::ColorEdit4("##AmbientColor", &s_Scene.ambientLight.color.x);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Ambient Intensity");
  ImGui::SameLine(labelWidth);
  ImGui::SliderFloat("##AmbientIntensity", &s_Scene.ambientLight.intensity, 0.0f, 2.f, "%.3f");

  ImGui::End();
}
