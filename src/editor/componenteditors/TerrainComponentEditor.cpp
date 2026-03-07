#include "TerrainComponentEditor.h"

#include <imgui.h>

#include "Scene.h"
#include "ServiceLocator.h"
#include "TerrainSystem.h"

void TerrainComponentEditor::OnInit() {
  auto& scene = ServiceLocator::Get<Scene>();
  myTerrainSystem = scene.mTerrainSystem.get();
  if(!myTerrainSystem)
    LOG_ERROR << "Could not get terrain system";
}

void TerrainComponentEditor::OnDrawComponent(void* target, ecs::Entity entity){
  TerrainComponent* terrain = static_cast<TerrainComponent*>(target);
  if(!terrain) return;

  // height map texture
  myHeightMap.renderImGui("Height Map");  
  if(myHeightMap.isDirty()){
    if(auto newTex = myHeightMap.create()){
      terrain->heightMap = newTex;
    }
  }

  // show size
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Size");
  ImGui::SameLine();
  ImGui::DragFloat3("##Size", &terrain->size.x);

  // show resolution
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Resolution");
  ImGui::SameLine();
  ImGui::DragInt("##Resolution", &terrain->resolution);

  

  // allow user to create terrain 
  if(terrain->heightMap){
    ImGui::Dummy(ImVec2(0, 5));
    if(ImGui::Button("Generate terrain")){
      myTerrainSystem->generateTerrain(entity);   
    }
  }
}
