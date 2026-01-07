#include "CameraComponentEditor.h"
#include "imgui.h"

void CameraComponentEditor::OnDrawComponent(Component* target){
  constexpr float labelWidth = 140.f;

  if(CameraComponent* cmp = dynamic_cast<CameraComponent*>(target)){
    // perpsective params
    ImGui::Text("Perspective Parameters");
    ImGui::Spacing();

    // draw fov
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Fov");
    ImGui::SameLine(labelWidth);
    ImGui::SliderFloat("##Fov", &cmp->cameraData.fov, 0.0f, 180.f,  "%.2f");

    // draw aspect ratio
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Aspect Ratio");
    ImGui::SameLine(labelWidth);
    ImGui::InputFloat("##Aspect Ratio", &cmp->cameraData.aspectRatio);
    
    // add some spacing
    ImGui::Dummy(ImVec2(0.0f, 20.f));

    // frustrum params
    ImGui::Text("Frustrum Parameters");
    ImGui::Spacing();

    // draw nearplane
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Near Plane");
    ImGui::SameLine(labelWidth);
    ImGui::InputFloat("##Near Plane", &cmp->cameraData.nearPlane);


    // draw nearplane
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Far Plane");
    ImGui::SameLine(labelWidth);
    ImGui::InputFloat("##Far Plane", &cmp->cameraData.farPlane);
  }

}
