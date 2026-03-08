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
      terrain->material->setTexture("uHeightMap", newTex);

      // force terrain system to regenerate this
      terrain->isDirty = true;
    }
  }

  // show resolution
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Resolution");
  ImGui::SameLine();
  if(ImGui::DragInt("##Resolution", &terrain->resolution)){
    // force terrain system to regenerate this
    terrain->isDirty = true;
  }
}
