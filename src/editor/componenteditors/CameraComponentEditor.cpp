#include "CameraComponentEditor.h"
#include "ServiceLocator.h"
#include "imgui.h"
#include "Scene.h"

void CameraComponentEditor::OnDrawComponent(void* target, ecs::Entity entity){
  constexpr float labelWidth = 140.f;

  if(CameraComponent* cmp = static_cast<CameraComponent*>(target)){

    // Camera main toggle
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Main Camera");
    ImGui::SameLine(labelWidth);
    if(ImGui::Checkbox("##Main Camera", &cmp->isMainCamera)){
      if(cmp->isMainCamera){
        Scene& mainScene = ServiceLocator::Get<Scene>();
        mainScene.SetActiveCamera(entity);
      }
    }
    
    // add some spacing
    ImGui::Separator();

    // draw fov
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Fov");
    ImGui::SameLine(labelWidth);
    ImGui::SliderFloat("##Fov", &cmp->cameraData.fov, 0.0f, 179.f,  "%.1f");

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
